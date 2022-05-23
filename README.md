# nesdev-template

A simple starter template for an NES project based around the ca65 assembler and
linker, and GNU Make. It includes:

* A working Makefile.
* A simple tool for automatically generating the Makefile-compatible build
  dependencies of each module by recursively scanning the assembly source for
  included files; the Makefile uses this by default.
* Simple tooling to generate character ROM binaries from PNG or XCF sprite
  sheets as part of the make process.
* A PPU module with working reset/control routines, an NMI handler that
  implements a simple system for buffered writes of all key graphics data: OAM
  data (via DMA), Nametable data (via a write log), the scroll/mask/control
  registers. Provides routines for interacting with the buffers and NMI handler.
* Some generic math macros derived from public domain code at 6502.org and
  cleaned up for ca65.
* A random number generation module adapted from Damian Yerrick's (see copyright
  notice in `asm/random.s`).
* A simple controller input module.
* A main module that ties these elements together into a simple game intended
  for NROM cartridges that displays a static graphical scene with a minimal game
  loop and input handling.

## Usage

TODO.
