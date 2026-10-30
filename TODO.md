# TODO

Ideas and planned work for this fork of libTAS.

## Build system

- [x] Upgrade from Qt5 to Qt6
- [x] Replace autotools with CMake

## SDL

- [x] Implement SDL2 timers to fix music fades in Sonic Robo Blast 2's SDL2 mixer backend
- [x] SDL2 OSD fix
- [ ] More SDL3 testing

## Platform support

- [ ] Native Wayland support
- [ ] Wine support (WIP in the `wine` branch)
- [ ] Revive macOS support

## Testing

- [ ] End-to-end tests (see upstream PR #733 for current progress)

## Debugging

- [x] Log the Windows steam_api.dll to a file (`<datadir>/steam_api.log`,
  following the log level of the game), as GUI games have no stderr under
  Wine
