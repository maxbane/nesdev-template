# GNU Makefile for template nesdev project.

# Project title, basename of final iNES container file.
PROJECT = template

# Location of the base directory of the cc65 distribution or repository; must
# contain a bin/ subdirectory where ca65 and ld65 are located.
CC65DIR = ../../../tools/cc65

# Project source/object tree
INCDIR		= inc
ASMDIR		= asm
CHRDIR		= chr

# Required linker configuration file, which controls the layout of code/data
# segments in the iNES container as well as the runtime address ranges of
# memory segments (code, data, RAM).
LDCFG	= ldcfg/nrom.cfg

# Output dirs (created by make, deleted by make clean)
OBJDIR		= obj
DISTDIR		= dist

# Target iNES container file to create
NESFILE = $(DISTDIR)/$(PROJECT).nes

# Default target
all: $(NESFILE)
.PHONY: all

# How to create the OBJDIR
$(OBJDIR):
	mkdir $(OBJDIR)

# (Note that the DISTDIR is created as part of the rule for nes files).

#
# Which object files should get linked together into the final iNES container.
#
OBJECTS = locals.o main.o ppu.o joy.o random.o

$(NESFILE): $(OBJECTS)

#
# Object file dependencies, other than the implicit %.s.
# Note that locals.o and ppu.o only depend on their .s files, hence omitted
# from this list.
#
main.o: 			locals.inc ines.inc ppu.inc joy.inc \
					math_macros.inc chr/sprites.chr \
					chr/background.chr random.inc
joy.o: 				locals.inc
random.o:			random.s random.inc

# Emulator locations
NESTOPIADIR 	= ../../emu/nestopia/nestopia-1.46.2
FCEUXDIR		= ../../emu/fceux/fceux-2.2.2-win32
NINTENDULATORDIR= ../../emu/ndx/nintendulatordx-v36
MESENDIR		= ../../emu/mesen-x

# Tool locations
CC65BINDIR	= $(CC65DIR)/bin
AS			= $(CC65BINDIR)/ca65
LD65		= $(CC65BINDIR)/ld65

# Assembler (ca65) flags, e.g., for debugging symbols
ASFLAGS = -g --include-dir $(INCDIR) --bin-include-dir $(CHRDIR)

# Optional linker (ld65) flags
LDFLAGS = --obj-path $(OBJDIR)


# clean: remove all generated files
RMDIRFLAGS=--ignore-fail-on-non-empty
clean:
	rm -f \
		obj/* \
		chr/*.png chr/*.chr \
		$(DISTDIR)/*
	test -d $(OBJDIR) && rmdir $(RMDIRFLAGS) $(OBJDIR) || true
	test -d $(DISTDIR) && rmdir $(RMDIRFLAGS) $(DISTDIR) || true
.PHONY: clean

#
# Print a table of how memory segments are laid out
#
summary: $(NESFILE)
	@cat $(<:.nes=.map.txt) | grep -A 9 "^Name"
.PHONY: summary

#
# Convenience targets to launch the game in emulators for testing
#
nestopia: 		$(NESFILE)
	$(NESTOPIADIR)/nestopia $< 2> /dev/null > /dev/null

nintendulator: 	$(NESFILE)
	$(NINTENDULATORDIR)/Nintendulator.exe $< 2> /dev/null

fceux: 			$(NESFILE)
	$(FCEUXDIR)/fceux.exe $< 2> /dev/null

mesen: 	$(NESFILE)
	$(MESENDIR)/Mesen.exe $< 2> /dev/null


.PHONY: nestopia nintendulator fceux mesen

#
# Implicit rules on how to build PNGs from XCFs, and CHRs from PNGs. A code
# module that .incbin's a CHR bank need only mention the CHR file in its
# prerequisites, and make will figure out how to generate it through the 
# XCF -> PNG -> CHR pipeline.
#
%.chr: %.png bin/bmp2nes
	bin/bmp2nes -i $< -o $@

%.png: %.xcf bin/xcf2png
	bin/xcf2png $< $@

# Don't delete the intermediate PNGs created by chaining the above implicit
# rules.
.PRECIOUS: %.png

# The main implicit rules for building object and nes files.

%.o: %.s $(OBJDIR)
	$(AS) $(ASFLAGS) -o $(OBJDIR)/$@ $<

%.nes: $(LDCFG)
	mkdir -p $$(dirname $@)
	$(LD65) $(LDFLAGS) -o $@ -C $(LDCFG) -m $*.map.txt \
		-Ln $*.labels.txt --dbgfile $$(echo $@ | sed 's/\.nes$$/.dbg/') \
		$(wordlist 2, $(words $^), $^)

vpath 	%.o 	$(OBJDIR)
vpath 	%.s 	$(ASMDIR)
vpath 	%.inc 	$(INCDIR)
vpath 	%.chr 	$(CHRDIR)


