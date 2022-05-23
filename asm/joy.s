; NES Gamepad module
; Useful constants and subroutines
; TODO: support multiple gamepads, efficiently (macros?)
.fileopt    comment, "NES gamepad module"
.fileopt    author,  "Max Bane"

.include "locals.inc"

.scope Joy

.export PAD0_ADDR = $4016
.export PAD1_ADDR = $4017

.segment "ZEROPAGE"
pad_0: .res 1
.exportzp pad_0
new_buttons_0: .res 1
.exportzp new_buttons_0

.segment "CODE"
; Joy::poll: this reads the gamepad state into the variable labelled "gamepad"
;   This only reads the first gamepad, and also if DPCM samples are played they can
;   conflict with gamepad reading, which may give incorrect results.
.export poll
.proc poll
    ; strobe the gamepad to latch current button state
    lda #1
    sta PAD0_ADDR
    lda #0
    sta PAD0_ADDR
    ; read 8 bytes from the interface at $4016
    ldx #8
    :
        pha
        lda PAD0_ADDR
        ; combine low two bits and store in carry bit
        and #%00000011
        cmp #%00000001
        pla
        ; rotate carry into gamepad variable
        ror
        dex
        bne :-
    sta pad_0
    rts
.endproc

.export store_new_buttons
.proc store_new_buttons
    ; last frame's joypad state
    last_frame_joy = local_0
    ; we'll need to read twice to account for the DPCM glitch. This holds our
    ; first read.
    first_read = local_1

    lda Joy::pad_0
    sta last_frame_joy

    jsr Joy::poll
    lda Joy::pad_0
    sta first_read
    jsr Joy::poll

    ; make sure the two reads agree
    lda Joy::pad_0
    cmp first_read
    beq :+
        ; reads disagreed.
        ; no input handling this frame. restore last frame's state as "current"
        lda last_frame_joy
        sta Joy::pad_0
        rts
    :

    ; determine new button depressions
    lda last_frame_joy  ; A = buttons that were down last frame
    eor #$ff            ; A = buttons that were up last frame
    and Joy::pad_0      ; A = buttons down now and up last frame 
    sta Joy::new_buttons_0
    rts
.endproc

.endscope ; Joy
