.include "random.inc"

; Derived from Damian Yerrick's random.s, which included the following notices:
;
;   Copyright 2014 Damian Yerrick
;
;   Copying and distribution of this file, with or without
;   modification, are permitted in any medium without royalty provided
;   the copyright notice and this notice are preserved in all source
;   code copies.  This file is offered as-is, without any warranty.
;
;   This is based on a routine by Greg Cook that implements
;   a CRC-16 cycle in constant time, without tables.
;   39 bytes, 66 cycles, AXP clobbered, Y preserved.
;   http://www.6502.org/source/integers/crc-more.html
;   Setting seed to $FFFF and then taking
;   CRC([$01 $02 $03 $04]) should evaluate to $89C3.


.segment "ZEROPAGE"

; Current value of CRC, not necessarily contiguous
Random::crc_lo: .res 1
Random::crc_hi: .res 1

.segment "CODE"

; If using CRC as a PRNG, use this entry point
Random::random_crc16 = random_crc16
.proc random_crc16
  lda #$00
  ; fall-through to crc16_update...
.endproc

Random::update_crc16 = update_crc16
.proc update_crc16
    eor Random::crc_hi  ; A contained the data
    sta Random::crc_hi  ; XOR it into high byte
    lsr                 ; right shift A 4 bits
    lsr                 ; to make top of x^12 term
    lsr                 ; ($1...)
    lsr
    tax                 ; save it
    asl                 ; then make top of x^5 term
    eor Random::crc_lo  ; and XOR that with low byte
    sta Random::crc_lo  ; and save
    txa                 ; restore partial term
    eor Random::crc_hi  ; and update high byte
    sta Random::crc_hi  ; and save
    asl                 ; left shift three
    asl                 ; the rest of the terms
    asl                 ; have feedback from x^12
    tax                 ; save bottom of x^12
    asl                 ; left shift two more
    asl                 ; watch the carry flag
    eor Random::crc_hi  ; bottom of x^5 ($..2.)
    sta Random::crc_hi  ; save high byte
    txa                 ; fetch temp value
    rol                 ; bottom of x^12, middle of x^5!
    eor Random::crc_lo  ; finally update low byte
    ldx Random::crc_hi  ; then swap high and low bytes
    sta Random::crc_hi
    stx Random::crc_lo
    rts
.endproc
