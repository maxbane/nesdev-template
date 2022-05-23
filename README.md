# nesdev-template

A simple starter template for an NES project based around the ca65 assembler and
linker, and GNU Make. It includes:

* A working Makefile that builds a stub game ready for fleshing out.
* A simple tool for automatically generating the Makefile-compatible build
  dependencies of each module by recursively scanning the assembly source for
  included files; the Makefile uses this by default.
* Simple tooling to generate character ROM binaries from PNG or XCF sprite
  sheets as part of the make process. The `bin/bmp2nes` tools is by Damian
  Yerrick (see copyright notice there).
* A PPU module with working reset/control routines, an NMI handler that
  implements a simple system for buffered writes of all key graphics data: OAM
  data (via DMA), Nametable data (via a write log), the scroll/mask/control
  registers. Provides routines for interacting with the buffers and NMI handler.
* Some generic math macros derived from public domain code at 6502.org and
  cleaned up for ca65.
* A random number generation module adapted from Damian Yerrick's code (see
  copyright notice in `asm/random.s`).
* A simple controller input module (`asm/joy.s`, using "joy" as in "joy stick"
  or "joy pad").
* A main module that ties these elements together into a simple program intended
  for NROM cartridges that displays a static graphical scene with a minimal
  "game loop" and some input handling, ready to be replaced and extended.

## Usage

Optional prerequisite: [The GIMP](https://www.gimp.org), for building graphics
ROM data from XCF-format source assets. However, an intermediate PNG file is
included in the repo in case you don't have gimp on the path.

1. Fork this repo.
2. Edit the Makefile to reflect the desired name of target iNES file, as well as
   the locations of tools like ca65 and emulators.
3. Run `make` to build the target iNES file; run `make mesen` to do the same and
   run the Mesen emulator on the result.
4. Begin modifying and extending the source code. As you add new modules, edit
   the Makefile to include their object file names (.o) in the definition of the
   OBJECTS variable, causing them to be linked into the target iNES file.

### A note about includes and graphics

`.include` statements in your ca65 assembly code are automatically detected so
that make knows which other files (usually .inc) the module object (.o file)
depends on.

`.incbin` statements are also detected and mark binary files as
dependencies of the module.

Make will attempt to create dependencies that don't exist (or are out of date)
if it knows how, and the Makefile includes rules for creating binary CHR data
from PNGs, and PNGs from Gimp XCF files. Therefore it is only necessary to put
an XCF file `foo.xcf` in your repo as the "source" asset for some graphics, then
`.incbin foo.chr` in your assembly code, and Make will automatically determine
that it can satisfy the need for `foo.chr` by creating `foo.png` from `foo.xcf`
and then `foo.chr` from `foo.png`, and that it must do so before assembling the
module. See the example graphics files (`chr/*.xcf`) for the pixel layout and
color format conventions expected by the graphics conversion tools.
