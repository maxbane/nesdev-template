; NES Gamepad module
; Useful constants and subroutines

.ifndef Joy_included
Joy_included = 1
.scope Joy
    .import PAD0_ADDR
    .import PAD1_ADDR

    ; button state masks
    BUTTON_A      = $01
    BUTTON_B      = $02
    BUTTON_SELECT = $04
    BUTTON_START  = $08
    BUTTON_UP     = $10
    BUTTON_DOWN   = $20
    BUTTON_LEFT   = $40
    BUTTON_RIGHT  = $80

    .importzp pad_0
    .importzp new_buttons_0
    .import   poll
    .import   store_new_buttons
.endscope ; Joy
.endif ; .ifndef Joy_included
