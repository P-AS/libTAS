# Agent instructions

## Changelog

Every change you make must be recorded in `CHANGELOG.md`, in the same commit
as the change itself. Do not wait to be asked.

* Add entries under the `## [Unreleased]` section at the top of the file.
* Pick the matching subsection: `### Added` (new features, hooks, options),
  `### Changed` (behavior changes to existing features) or `### Fixed`
  (bug fixes). Create the subsection if it is missing, keeping the order
  Added, Changed, Fixed.
* Append the entry at the end of the subsection's list.
* Follow the existing style:
  * One bullet per change, starting with `* ` and a capitalized verb or
    subject (e.g. `* Add hooks for Unity 2023`, `* Fix SDL_RenderSetLogicalSize being called with 0s`).
  * Keep it short, a single line where possible, with no trailing period.
  * Wrap long entries at around 80 columns, indenting continuation lines by
    two spaces.
  * Prefix with the subsystem when helpful (e.g. `* Vulkan: ...`,
    `* aarch64: ...`, `* Steam: ...`, `* Input editor: ...`).
  * Reference GitHub issues/PRs in parentheses when relevant, e.g. `(#741)`
    or `(fix #745)`.
* Describe the user-visible effect, not implementation details.
* Do not edit entries of already released versions.
