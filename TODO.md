# TODO

Ideas and planned work for this fork of libTAS.

## Build system

- [x] Upgrade from Qt5 to Qt6
- [x] Replace autotools with CMake

## SDL

- [x] Implement SDL2 timers to fix music fades in Sonic Robo Blast 2's SDL2 mixer backend
- [x] SDL2 OSD fix
- [ ] More SDL3 testing
- [ ] SDL2 via sdl2-compat support
- [ ] SDL 1.2 via sdl12-compat support

## Platform support

- [ ] Wine support (WIP in the `wine` branch)
- [ ] Native Wayland support
- [ ] Revive macOS support

## Movie Authoring

- [ ] Ability to 'lock' columns of inputs, for 2P TAS runs
- [ ] Saving and loading branches inputs in ltm file (upstream PR #711)
- [ ] Lua: Add Ramsearch functions (upstream PR #687)

## Testing

- [ ] End-to-end tests (see upstream PR #733 for current progress)
