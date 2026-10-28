---
layout: page
title: RNG sources
permalink: /guides/rng/
---

This page lists possible sources that games may use for their Random Number Generator (RNG), and which options control them in libTAS.

* TOC
{:toc}

## Sources

Besides the [regular in-game sources of randomness](https://tasvideos.org/LuckManipulation), games may get data from other sources to update their PRNG. Some of these sources are configurable, while libTAS provides fixed values for others.

For more information about these different sources, see [the Tech details page](../how).

### Configurable

* System time: can be configured on the main window
* Reading `/dev/urandom` or calling `getrandom()`: libTAS returns data from a
  deterministic PRNG seeded with the initial system time, so it changes along
  with the system time

### Currently not configurable

* Reading `/proc/cputime`
* Using the current process pid
