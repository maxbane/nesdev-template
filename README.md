# nesdev-template

A simple starter template for an NES project based around the ca65 assembler and
linker, and GNU Make. It includes:

* A working Makefile
* Simple tooling for generating character ROM binaries from PNG or XCF sprite
  sheets as part of the build process.
* A PPU module with working reset/control routines, an NMI handler that
  implements a simple system for buffered writes of OAM data, Nametable data,
  PPU scroll/mask/control registers, and routines for interacting with the
  buffers and NMI handler.
* Some generic math macros.
* A random number generation module.
* A simple controller input module.
* A main module that ties these elements together into a simple game intended
  for NROM cartridges that displays a static graphical scene with a minimal game
  loop and input handling.

## Usage

TODO.
