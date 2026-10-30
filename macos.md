# macOS dlopen/dlsym Hook Implementation — Investigation & Work Log

## Objective
Get Sonic Robo Blast 2 (SRB2 v2.2.15) running through libTAS on macOS using `DYLD_INSERT_LIBRARIES` by fixing symbol resolution for dlopen hooks.

## Environment
- macOS 27.2 (26B123), Apple Silicon (arm64e)
- SRB2: native arm64, SDL2, uses dlopen/dlsym — crashes during init
- All changes uncommitted

## Key Decisions & Findings

### dlopen_from vs dlsym
- **dlopen_from**: Weak function from dyld that bypasses inserted library scope — works for dlopen
- **RTLD_DEFAULT does NOT bypass** the hooked library on macOS
- **No equivalent `_dl_sym` on macOS** — cannot get original `dlsym` from within hook
- **dlopen_from(RTLD_NOLOAD) crashes** with SIGILL during early dyld init — cannot use during init
- **dlopen_from crashes with SIGILL** when called during symbol resolution — needs fallback to mmap-based parsing

### getenv Wrapper
- `getenv` wrapper uses `_NSGetEnviron()` directly, no `link_function`
- On macOS, `getenv()` calls `link_function` which calls `dlsym` which calls `find_sym` — infinite recursion
- `_NSGetEnviron()` accessed via `#include <crt_externs.h>`

### Symbol Resolution Architecture
- `find_sym` uses `getenv_direct_call()` to avoid recursion (from dlreal.cpp)
- `find_sym` has recursion guard: `finding_sym` thread-local pointer
- `g_resolve_depth` thread-local recursion guard prevents `open` → `resolve_native_symbol` → `find_symbol_in_file` → `open` loop
- `hooks_enabled` thread-local flag controls when hooks activate (true after constructor 250)
- Constructor priorities: 150 = `collect_headers_before_init`, 250 = enable hooks + `libtas::init`

### Raw Syscalls
- `open` wrapper with init fallback uses raw syscalls to bypass hooked open
- `syscall` function is NOT hooked (it's in libsystem_kernel.dylib)
- `SYS_open=5`, `SYS_fstat64=339`, `SYS_close=6` on macOS arm64
- Raw syscalls used in `find_symbol_in_file` to open library files from disk

### native_strcmp
- Used in symbol resolution to avoid hooked `strcmp`
- Only compares ASCII strings byte-by-byte
- Critical for Mach-O symbol table iteration without recursion

## Mach-O Symbol Resolution Details

### parse_in_memory — Runtime Header Parsing
Parses Mach-O headers at runtime addresses (from `_dyld_get_image_header()`):
- Load commands start at `hdr + sizeof(struct mach_header_64)` = 32 bytes offset
- Uses `memcpy` for all field reads on ARM64 to avoid SIGBUS from unaligned access
- Symbol addresses computed as `section.addr + n_value` where section.addr is already virtual address
- `struct nlist_64` fields accessed directly via `memcpy`

### find_symbol_in_file — mmap'd File Parsing
Parses Mach-O files from disk using mmap'd memory:

1. **Fat binary handling**: All `fat_arch` fields are BIG-ENDIAN in the file
   - `cputype`, `cpusubtype`, `align`, `offset`, `size` all need `OSSwapBigToHostInt32()`
   - Verified with Python: x86_64 at offset 0x4000, arm64 at 0x20000

2. **Section parsing**: section.addr in mmap'd file is NOT the same as runtime vmaddr
   - mmap'd `__text` section.addr = 0x870 (file-relative, NOT vmaddr)
   - Runtime `__text` section.addr = 0x180586958 (actual vmaddr)
   - The mmap'd file offset and section.addr are FILE OFFSETS, not virtual addresses

3. **Address formula problem**: Cannot convert mmap'd section.addr to runtime addresses
   - Tested: `slide + mmap_sa + nv` → WRONG (0x1d46ac38 vs actual 0x19d9f8764)
   - Tested: `slide + runtime_section.addr + nv` → WRONG (0x19d9f0d20 vs actual 0x19d9f8764)
   - Tested: `runtime_hdr + nv` → WRONG (0x19d9f03c8 vs actual 0x19d9f8764)
   - The mmap'd file doesn't contain enough information to compute runtime addresses

### _pthread_self Verification
Tested with `/usr/lib/system/libsystem_pthread.dylib`:
- Actual dlsym result: 0x19d9f8764
- nm reports: `_pthread_self = 0x23c8` (N_SECT entry, sect=1)
- Also found: `_pthread_self.cold.1 = 0xab68` (cold variant, must skip)
- `_pthread_self_is_exiting_np = 0x7cac`
- File: contains x86_64, arm64, arm64e, arm64e.x1 slices (via `lipo -info`)

### dyld Runtime Info
- `_dyld_get_image_header()` returns valid runtime pointer (hdr=0x19d9ee000)
- dyld slide = 0x1d468000
- `__TEXT` vmaddr = 0x180586000 (from runtime header)
- `__text` section.addr = 0x180586958 (from runtime header)
- `hdr - slide = 0x180586000` = matches `__TEXT` vmaddr

### Fat Binary Field Sizes
`struct fat_arch` on arm64 macOS is 20 bytes:
- cputype: uint32_t (BE)
- cpusubtype: uint32_t (BE)
- offset: uint32_t (BE)
- size: uint32_t (BE)
- align: uint32_t (BE)

## Files Modified

### src/library/general/dlhook.cpp (core changes)
- Added `native_strcmp()` and `native_strstarts()` — avoid hooked strcmp
- Added `parse_in_memory()` — parse Mach-O headers at runtime addresses
- Added `find_symbol_in_file()` — mmap file, parse fat binary, find symbols
- Added `resolve_path_symbol()` — try multiple system library paths
- Added `resolve_native_symbol()` — file-based symbol resolution for native calls
- Added `resolve_libsymbol()` — dlopen_from(RTLD_NOLOAD) + fallback for game libraries
- Added `getenv_direct_call()` usage in find_sym
- Added `g_resolve_depth` recursion guard (thread_local)
- Added `hooks_enabled` flag (thread_local, set true at constructor 250)
- Modified `get_dlfct_symbols()`: macOS now sets orig::dlopen = nullptr, orig::dlsym = nullptr
- Modified `dlopen()`: uses `dlopen_from` weak symbol on macOS
- Modified `dlsym()`: uses `find_sym()` path on macOS for native mode
- Modified `find_sym()`: uses `getenv_direct_call()`, handles macOS path

### src/library/general/dlhook.h
- Added `extern thread_local bool hooks_enabled`
- Added `resolve_native_symbol(const char *name)` declaration
- Added `resolve_libsymbol(const char *name, const char *library_hint)` declaration

### src/library/hook.cpp
- Modified `link_function()`: macOS now uses `resolve_libsymbol()` + `resolve_native_symbol()`
- Removed old TASK_DYLD_INFO-based library scanning (replaced by Mach-O parsing)
- Added `#ifdef __APPLE__` guards around macOS-specific paths

### src/library/fileio/posixiowrappers.cpp
- Modified `open()`: added `hooks_enabled` check for init-time fallback
- During init, calls `resolve_native_symbol("open")` as fallback
- Uses `dlopen_from` bypass when available

### src/library/general/localewrappers.cpp
- Modified `getenv()`: uses `_NSGetEnviron()` directly on macOS
- Iterates environment variables manually to avoid hook recursion

### src/dyld_func_lookup_helper/dyld_func_lookup_helper.cpp
- Added documentation explaining macOS symbol resolution strategy

### src/library/CMakeLists.txt
- Added `general/dlreal.cpp` to build sources

### New files: src/library/general/dlreal.cpp, dlreal.h
- Provides `getenv_direct_call()` using `_NSGetEnviron()`
- Bypasses the getenv wrapper for safe environment access during symbol resolution

## Address Formula Investigation (Unresolved)

We discovered that mmap'd file section.addr values are file offsets, not virtual addresses. This makes it impossible to compute runtime addresses from file parsing alone.

### Data points:
| Source | section.addr | Note |
|--------|-------------|------|
| mmap'd file | 0x870 | file offset |
| Runtime (dyld) | 0x180586958 | virtual address |
| dyld slide | 0x1d468000 | |
| __TEXT vmaddr (runtime) | 0x180586000 | |

### Tested formulas (all failed):
1. `slide + mmap_sa + n_value` → 0x1d46ac38 (expected 0x19d9f8764)
2. `slide + runtime_sa + n_value` → 0x19d9f0d20 (expected 0x19d9f8764)
3. `runtime_hdr + n_value` → 0x19d9f03c8 (expected 0x19d9f8764)
4. `slide + text_vmaddr + section_vm_offset + n_value` → still off by 0x4ad8

### Key insight:
The `section.addr` field in the Mach-O file represents the **file offset** of the section data, not its virtual memory address. This is because the file is thin (the __TEXT segment starts at file offset 0x0 with vmaddr 0x0). The actual vmaddr is determined by the dyld slide at load time.

### Working approach:
Use `_dyld_get_image_header()` which returns the **runtime** Mach-O header where section.addr is already the correct virtual address. Combined with the dyld slide, we can compute addresses correctly. The fallback for libraries not yet loaded is limited — file-based parsing cannot produce runtime addresses.

## Test Programs Created
- `/tmp/test_slide16.c` through `/tmp/test_slide24.c` — progressive experiments
- Each tested different combinations of slide, section.addr, and n_value
- Key verification: fat_arch fields are big-endian (confirmed by Python and C tests)
- Key verification: `nm -n` output matches Mach-O n_value for N_SECT symbols

## Current Limitations
1. **File-based symbol resolution cannot produce runtime addresses** — the mmap'd file lacks vmaddr information
2. **dlopen_from crashes** during early dyld init — cannot use for all symbol resolution
3. **Symbol resolution works via runtime headers** when libraries are loaded (`_dyld_get_image_header()` + slide)
4. **The formula `slide + text_vmaddr + nv`** gives wrong results because section.addr in file is offset, not vmaddr

## Potential Next Steps (if needed)
1. Use `_dyld_get_image_header()` for ALL symbol resolution (requires library to be loaded)
2. Compute slide from `header_ptr - text_vmaddr` for loaded libraries
3. For unloaded libraries, accept that symbol resolution may fail and rely on lazy binding
