# macOS Support — Design Decisions, Known Issues & Workarounds

> **Read this file before any macOS-related work.** It documents problems
> found, how they were fixed, design choices, debugging findings, and
> what is still open. Update it whenever you learn something new.

## Environment

- macOS 27.2 (26B123), Apple Silicon (arm64e)
- Target: SRB2 v2.2.15 (native arm64, SDL2, uses dlopen/dlsym)
- Injection: `DYLD_INSERT_LIBRARIES`
- Build: CMake with native macOS support (`805cc6fb`)

---

## Architecture & Design Decisions

### Hook Activation Guard — `hooks_enabled` (thread_local)

Hooks must not activate during early libTAS init, before libc is fully
initialized. Hooks activate only after `hooks_enabled` is set to true
in `enable_hooks()` at constructor priority 250. Priority 150 runs
`collect_headers_before_init`.

```
priority 150: collect_headers_before_init
priority 250: enable_hooks() → hooks_enabled = true
```

**Commit: 78f50111**

### `dlopen_from` — Bypassing Inserted Library Scope

`dlopen_from` is a **weak function from dyld** that bypasses the scope
of inserted libraries. It works for dlopen on macOS where `dlsym`
resolution is more complicated.

- **Does work**: `dlopen_from(path, mode)` for normal dlopen
- **Does NOT work**: `dlopen_from(RTLD_DEFAULT, ...)` — undefined
- **Crashes**: `dlopen_from(RTLD_NOLOAD)` with SIGILL during early dyld init
- **Crashes**: `dlopen_from` called during symbol resolution (SIGILL)

`RTLD_DEFAULT` does **not** bypass the hooked library on macOS — it
still resolves through the injected libTAS library.

**No equivalent `_dl_sym` exists on macOS** — cannot get the original
`dlsym` from within a hook. Must use a different strategy.

**Commit: 78f50111**

### `find_sym()` — Dyld Image Scanning with Handle-Aware Resolution

On macOS, `orig::dlsym` is always null (we use `dlopen_from` for dlopen).
The original `find_sym()` simply returned nullptr without trying to
resolve any symbols. This broke SRB2's dlopen/dlsym usage.

**Fix**: Scan all loaded dyld images (`_dyld_get_image_header()` +
`_dyld_get_image_name()`) and parse their Mach-O symbol tables via
`parse_in_memory()` (already uses `memcpy` for ARM64 alignment safety).

- **`handle == nullptr` (RTLD_DEFAULT)**: Scan all loaded images.
  - Native mode: return any found symbol (library or libTAS).
  - Wine mode: only return libTAS symbols (hook interception).
- **`handle != nullptr` (specific library)**: Skip libTAS symbols
  and set addr = nullptr, letting `find_sym_native()` handle library-scoped
  lookup by matching the handle against loaded dyld images.

`find_sym()` signature changed: `void *find_sym(const char *name, void *handle = nullptr)`.

**Commit: a96eeb47**

### getenv Bypass — `getenv_direct_call()`

On macOS, `getenv()` is hooked and calls `link_function` → `dlsym` →
`find_sym`, causing infinite recursion.

**Fix**: Use `_NSGetEnviron()` directly from `crt_externs.h` to access
the environment pointer without going through the hooked `getenv`.
This is implemented in `dlreal.cpp` as `getenv_direct_call()`.

```cpp
#include <crt_externs.h>
// _NSGetEnviron() returns environ* directly
```

The wrapper iterates environment variables manually.

**Commit: 78f50111**

### Recursion Guards — `finding_sym` + `g_resolve_depth`

Two layered guards prevent infinite recursion in symbol resolution:

1. **`finding_sym`** (thread_local pointer): Set in `find_sym()` before
   attempting resolution. Checked at the top of `find_sym()` — if
   non-null, returns immediately to break `dlsym` → `find_sym` loops.

2. **`g_resolve_depth`** (thread_local int): Prevents `open` →
   `resolve_native_symbol` → `find_symbol_in_file` → `open` loops.
   File-based symbol resolution calls `open()` to read library files
   from disk.

**Commit: 78f50111**

### `native_strcmp` — Avoiding Hooked strcmp

Symbol resolution iterates Mach-O symbol tables byte-by-byte. A hooked
`strcmp` would cause recursion. `native_strcmp()` compares ASCII
strings byte-by-byte using raw memory reads — no function call
abstraction, no libc dependency.

Only compares ASCII strings (Mach-O symbol names are always ASCII).

**Commit: 78f50111**

### Raw Syscalls for File I/O

`open` is hooked. Raw syscalls bypass the hook to open library files
for symbol table parsing:

| Syscall    | Number (macOS arm64) |
|------------|---------------------|
| `SYS_open` | 5                   |
| `SYS_fstat64` | 339              |
| `SYS_close` | 6                  |

The `syscall` function itself is NOT hooked — it lives in
`libsystem_kernel.dylib`.

**Commit: 78f50111**

### Mach-O Runtime Parsing — `memcpy` for Unaligned Access

On ARM64, unaligned memory accesses cause SIGBUS. All Mach-O field
reads must use `memcpy` into a local variable before use, even for
naturally-aligned fields. This applies to both runtime header parsing
(`parse_in_memory`) and file parsing (`find_symbol_in_file`).

**Commit: 78f50111**

### Fat Binary — Big-Endian Field Ordering

Universal binaries (fat Mach-O) store `fat_arch` fields in **big-endian**
byte order in the file, regardless of the host CPU's endianness. All
fields need `OSSwapBigToHostInt32()`:

- `cputype`, `cpusubtype`, `align`, `offset`, `size` — all uint32_t BE
- `struct fat_arch` is 20 bytes on arm64

Verified with Python analysis and C tests against known fat binaries.

**Commit: 78f50111**

### Symbol Name — Leading Underscore Convention

Mach-O string tables prefix all C symbol names with an underscore.
Lookup must try `_`-prefixed variants:

```
pthread_self  →  _pthread_self  (in libsystem_pthread.dylib)
```

`resolve_path_symbol()` tries both variants automatically.

**Commit: 78f50111**

---

## Resolved Problems

### _NSGetExecutablePath Crash on macOS
`/proc/self/exe` does not exist on macOS. Crash during init when
libTAS tried to read the executable path.

**Fix**: Use `_NSGetExecutablePath()` (macOS API) instead of
`readlink("/proc/self/exe", ...)`.

**Commit: 61f96ab6**

### `#ifdef __unix__` → `#ifdef __linux__`
Several files used `__unix__` which is true on macOS, causing
compilation errors or runtime issues. All Linux-specific code must be
wrapped in `__linux__`.

**Fix**: Replaced `__unix__` with `__linux__` in 12 files. Wrapped
X11/XCB functions in `SaveStateManager.cpp`.

**Commits: c9640e24, 0f0ba0b9**

### build.sh — AppImage Step on macOS
The AppImage build step failed on macOS (no `appimagetool`).

**Fix**: Skip AppImage step on non-Linux.

**Commit: b8b2f861**

### Infinite Recursion in find_sym
`find_sym()` called `getenv()` which was hooked, triggering
`dlsym` → `find_sym` → infinite loop.

**Fix**: `getenv_direct_call()` using `_NSGetEnviron()` bypasses the
wrapper entirely.

**Commit: 78f50111**

### Symbol Resolution via _dyld_get_image_header()
Runtime Mach-O header parsing works correctly:
- `_dyld_get_image_header()` returns valid runtime pointers
- `hdr - slide = __TEXT vmaddr` (verified)
- Section `.addr` in runtime headers is already the correct virtual address
- Combined with dyld slide, addresses compute correctly

**Working formula**: `runtime_section.addr + n_value` gives correct
runtime addresses for loaded libraries.

**Commit: 78f50111**

### mmap'd File Parsing — Big-Endian fat_arch
Fat binary file parsing initially failed due to incorrect byte-order
handling of `fat_arch` fields.

**Fix**: All `fat_arch` fields are big-endian in the file. Applied
`OSSwapBigToHostInt32()` to each field.

**Commit: 78f50111**

---

## Open Problems

### Socket Accept Timeout — `initSocketGame()` 30-second limit

On macOS, the game may crash during early initialization (before `initSocketGame()` 
finishes setting up the socket), causing the program to wait indefinitely on `accept()`.

**Fix**: Added 30-second `SO_RCVTIMEO` timeout to the listening socket. Also removed
stale socket file before binding, and added error logging for bind/listen failures.

**Commit: c72eeb47**

### File-Based Address Resolution Cannot Produce Runtime Addresses

When parsing Mach-O files from disk via mmap, `section.addr` values in
the mapped memory are **file offsets** (e.g., 0x870), not virtual
memory addresses (e.g., 0x180586958). There is no reliable formula to
convert file offsets to runtime addresses.

Tested and rejected:
1. `slide + mmap_section.addr + n_value` — wrong (0x1d46ac38 vs 0x19d9f8764)
2. `slide + runtime_section.addr + n_value` — wrong (off by 0x4ad8)
3. `runtime_hdr + n_value` — wrong
4. `slide + text_vmaddr + section_vm_offset + n_value` — off by 0x4ad8

**Current workaround**: Use `_dyld_get_image_header()` for loaded
libraries (where section.addr is already the correct vmaddr). File-based
parsing serves as fallback for name-only resolution.

**Unresolved** since 78f50111.

### dlopen_from Crashes with RTLD_NOLOAD

`dlopen_from(RTLD_NOLOAD, path)` crashes with SIGILL when called during
early dyld initialization. Cannot be used for all symbol resolution.

**Workaround**: Fall back to file-based Mach-O parsing for libraries
not yet loaded. Use `_dyld_get_image_header()` for already-loaded
libraries.

**Unresolved** since 78f50111.

### No Original dlsym Equivalent

macOS has no `_dl_sym` or similar function to retrieve the original
(unhooked) `dlsym`. All symbol resolution must be implemented manually
via Mach-O parsing.

**Unresolved** — this is a fundamental platform limitation.

---

## File Layout

### src/library/general/dlhook.cpp
Core symbol resolution. See macos.md individual sections for details.

- `native_strcmp()` / `native_strstarts()` — avoid hooked strcmp
- `parse_in_memory()` — parse Mach-O at runtime addresses
- `find_symbol_in_file()` — mmap file, parse fat binary, find symbols
- `resolve_path_symbol()` — try multiple system library paths
- `resolve_native_symbol()` — file-based symbol resolution
- `resolve_libsymbol()` — dlopen_from(RTLD_NOLOAD) + fallback
- `g_resolve_depth` — recursion guard (thread_local)
- `hooks_enabled` — hook activation flag (thread_local)
- `find_sym(handle)` — scans `_dyld_get_image_header()` for RTLD_DEFAULT;
  matches handle against dyld images for library-scoped dlsym
- `find_sym_native(handle)` — for non-RTLD_DEFAULT, matches handle to
  a dyld image and parses that image's symbol table

### src/library/general/dlhook.h
Declarations for `find_sym()` (now takes `void *handle` parameter for
library-scoped resolution), `resolve_native_symbol`, `resolve_libsymbol`,
`hooks_enabled`.

### src/library/hook.cpp
`link_function()` — macOS uses `resolve_libsymbol()` +
`resolve_native_symbol()`. Removed old TASK_DYLD_INFO-based scanning.

### src/library/fileio/posixiowrappers.cpp
`open()` — hooks_enabled check + init fallback to
`resolve_native_symbol("open")`. Uses dlopen_from bypass.

### src/library/general/localewrappers.cpp
`getenv()` — uses `_NSGetEnviron()` directly on macOS.

### src/library/general/dlreal.cpp
`getenv_direct_call()` — uses `_NSGetEnviron()` to bypass hooked
getenv. No link_function dependency.

### src/library/general/dlreal.h
Declaration for `getenv_direct_call()`.

### src/dyld_func_lookup_helper/dyld_func_lookup_helper.cpp
Documentation explaining macOS symbol resolution strategy.

### src/library/CMakeLists.txt
Added dlreal.cpp to build sources.

---

## Debugging Techniques Worth Remembering

- **fat_arch fields are big-endian**: Confirmed via Python analysis
  and C tests against `/usr/lib/system/*.dylib` fat binaries.
- **_pthread_self in libsystem_pthread.dylib**: N_SECT entry (type=0xf),
  sect=1 (__text), n_value=0x23c8. Also has cold variants (e.g.,
  `_pthread_self.cold.1`) that must be skipped during lookup.
- **nm -n output matches Mach-O n_value**: Verified `nm -n` output
  against parsed Mach-O symbol tables.
- **lipo -info confirms fat binary slices**: Use `lipo -info` to see
  available architectures in a universal binary.
- **Runtime section.addr offsets**: `struct mach_header_64` = 32 bytes
  before load commands. On arm64, vmaddr is at offset 16 (u64), nsects
  at offset 64 (u32), sections at offset 72 from load command start.
