---
layout: home
---

libTAS is a software for Linux that provides TAS tools to games, such as
frame advance, inputs recording, savestates. It is not a Linux emulator as
games are running natively on the user system, but it creates an intermediate
layer between the game and the operating system, feeding the game with
altered data (such as inputs, system time). We can call such tool a
**trans-layer** - an API translation layer. It tries to make the game running
deterministically, although it is still an issue when dealing with
multithreaded games. This layer connects to an external program to provide
a graphical interface to the user, with tools such as input editor or RAM
watch/search.

# Download

* **[Latest stable release, deb package](https://github.com/clementgallet/libTAS/releases/latest)**
* [Latest interim builds, deb package and AppImage (x86_64 and aarch64)](https://github.com/P-AS/libTAS/actions/workflows/build.yml)



 
