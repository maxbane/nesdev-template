.ifndef INES_included
INES_included = 1

; iNES header macro
.macro INES_HEADER INES_PRG_BANK_COUNT, INES_CHR_BANK_COUNT, INES_MAPPER, INES_MIRROR, INES_SRAM
    .ifblank INES_PRG_BANK_COUNT
        .error "INES_HEADER: required argument INES_PRG_BANK_COUNT not provided."
    .endif
    .ifblank INES_CHR_BANK_COUNT
        .error "INES_HEADER: required argument INES_CHR_BANK_COUNT not provided."
    .endif
    .ifblank INES_MAPPER
        .error "INES_HEADER: required argument INES_MAPPER not provided."
    .endif
    .ifblank INES_MIRROR
        .error "INES_HEADER: required argument INES_MIRROR not provided."
    .endif
    .ifblank INES_SRAM
        .error "INES_HEADER: required argument INES_SRAM not provided."
    .endif

    .byte 'N', 'E', 'S', $1A  ; Magic word
    .byte INES_PRG_BANK_COUNT ; 16k PRG bank count
    .byte INES_CHR_BANK_COUNT ; 8k CHR bank count
    .byte INES_MIRROR | (INES_SRAM << 1) | ((INES_MAPPER & $f) << 4)
    .byte (INES_MAPPER & %11110000)
    .byte $0, $0, $0, $0, $0, $0, $0, $0 ; padding
.endmacro

.endif ; INES_included
