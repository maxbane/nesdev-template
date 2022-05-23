; TODO: support immediate mode arguments where it makes sense. See ca65 doc
; example with .match

.ifndef math_macros_included
math_macros_included = 1

;------------------------------------------------
; Basic Operations
;------------------------------------------------

; Clear 2 bytes of memory at any location by
; setting it to zero.
;
; On exit: A = ??, X & Y are unchanged.
.macro mathmac_clr16 addr0, addr1, addr2
    .if .PARAMCOUNT < 1
        .error "mathmac_clr16: must provide at least one destination address"
    .endif
    lda #0
    sta addr0 + 0
    sta addr0 + 1
    .ifnblank addr1
        sta addr1 + 0
        sta addr1 + 1
    .endif
    .ifnblank addr2
        sta addr2 + 0
        sta addr2 + 1
    .endif
.endmacro


; Set the value of one, two, or three 16 bit locations with the given constant
; value.
;
; On exit: A = ??, X & Y unchanged.
.macro mathmac_set16 value, addr0, addr1, addr2
    .if .PARAMCOUNT < 2
        .error "mathmac_set16: must provide at least one destination address"
    .endif
    ; User must provide value as an immediate mode argument, for consistency
    .if (.match (.left (1, {value}), #))
        .if (.right (.tcount ({value})-1, {value})) <> 0
            ;lda #<value
            lda #<(.right (.tcount ({value})-1, {value}))
            sta addr0 + 0
            .ifnblank addr1
                sta addr1 + 0
            .endif
            .ifnblank addr2
                sta addr2 + 0
            .endif
            ;lda #>value
            lda #>(.right (.tcount ({value})-1, {value}))
            sta addr0 + 1
            .ifnblank addr1
                sta addr1 + 1
            .endif
            .ifnblank addr2
                sta addr2 + 1
            .endif
        .else
            mathmac_clr16 addr0, addr1, addr2
        .endif
    .else
        .error "mathmac_set16: value argument must be in immediate mode."
    .endif
.endmacro

;------------------------------------------------
; Logical Operations
;------------------------------------------------

; Calculate the logical NOT of the 16 bit value
; at location VLA and stores it in location RES.
;
; On exit: A = ??, X & Y are unchanged.
;
; example:
;   matchmac_not16 $1002,  $1004
;   matchmac_not16 #$abcd, $1000
.macro mathmac_not16    valueOrAddr, resultAddr
    ; User may provide an immediate mode value
    .if (.match (.left (1, {value}), #))
        ; valueOrAddr is a 16-bit immediate mode constant
        lda #<(.bitnot (.right (.tcount ({valueOrAddr})-1, {valueOrAddr})))
        sta resultAddr + 0
        lda #>(.bitnot (.right (.tcount ({valueOrAddr})-1, {valueOrAddr})))
        sta resultAddr + 1
    .else
        ; valueOrAddr is an address
        lda valueOrAddr + 0
        eor #$FF
        sta resultAddr  + 0
        lda valueOrAddr + 1
        eor #$FF
        sta resultAddr  + 1
    .endif
.endmacro

;------------------------------------------------
; Shift Operations
;------------------------------------------------

; Perform an arithmetic shift left on the 16 bit
; number at location VLA and store the result at
; location RES. If VLA and RES are the same then
; the operation is applied directly to the memory
; otherwise it is done in the accumulator.
;
; On exit: A = ??, X & Y are unchanged.
.macro mathmac_asl16 valueAddr, resultAddr
    .if (valueAddr <> resultAddr)
        lda valueAddr  + 0
        asl A
        sta resultAddr + 0
        lda valueAddr  + 1
        rol A
        sta resultAddr + 1
    .else
        asl valueAddr + 0
        rol valueAddr + 1
    .endif
.endmacro

;------------------------------------------------
; Arithmetic Operations
;------------------------------------------------

; Increment the 16 bit value at location MEM
; by one.
;
; On exit: A, X & Y are unchanged.
.macro mathmac_inc16 addr
    .local done
    inc addr + 0
    bne done
    inc addr + 1
    done:
.endmacro

; Decrement the 16 bit value at location MEM
; by one.
;
; On exit: A = ??, X & Y are unchanged.
.macro mathmac_dec16 addr
    .local done
    lda addr + 0
    bne done
    dec addr + 1
    done:
        dec addr + 0
.endmacro

; Add two 16 bit numbers together and store the result in another memory
; location. The first argument may be an address or an immediate mode value.
;
; On exit: A = ??, X & Y are unchanged.
.macro mathmac_add16 valueOrAddr0, valueAddr1, resultAddr
    clc
    .if (.match (.left (1, {valueOrAddr0}), #))
        lda #<(.right (.tcount ({valueOrAddr0})-1, {valueOrAddr0}))
    .else
        lda valueOrAddr0 + 0
    .endif
    adc valueAddr1 + 0
    sta resultAddr + 0

    .if (.match (.left (1, {valueOrAddr0}), #))
        lda #>(.right (.tcount ({valueOrAddr0})-1, {valueOrAddr0}))
    .else
        lda valueOrAddr0 + 1
    .endif
    adc valueAddr1 + 1
    sta resultAddr + 1
.endmacro

; Add two 16 bit numbers together and store the result in another memory
; location. The first argument may be an address or an immediate mode value.
; All addresses have the X index register added to them.
;
; On exit: A = ??, X & Y are unchanged.
.macro mathmac_add16_xoff valueOrAddr0, valueAddr1, resultAddr
    clc
    .if (.match (.left (1, {valueOrAddr0}), #))
        lda #<(.right (.tcount ({valueOrAddr0})-1, {valueOrAddr0}))
    .else
        lda valueOrAddr0 + 0, X
    .endif
    adc valueAddr1 + 0, X
    sta resultAddr + 0, X

    .if (.match (.left (1, {valueOrAddr0}), #))
        lda #>(.right (.tcount ({valueOrAddr0})-1, {valueOrAddr0}))
    .else
        lda valueOrAddr0 + 1, X
    .endif
    adc valueAddr1 + 1, X
    sta resultAddr + 1, X
.endmacro

; Negate the signed 16-bit number at location valueAddr0 and store the result
; at location resultAddr.
; On exit: A = ??, X & Y are unchanged.
.macro mathmac_neg16 valueAddr0, resultAddr
    sec
    lda #0
    sbc valueAddr0 + 0
    sta resultAddr + 0
    lda #0
    sbc valueAddr0 + 1
    sta resultAddr + 1
.endmacro

; Compute A % modulus
.macro mathmac_mod8 modulus
    .local dosub
    sec
    dosub:
    sbc modulus
    bcs dosub
    adc modulus
.endmacro

.endif ; math_macros_included
