.ifndef PPU_included
PPU_included = 1

.scope PPU
    ; PPU Register Address Constants
    .import REG_CTRL
    .import REG_MASK
    .import REG_STATUS
    .import REG_OAM_ADDR
    .import REG_OAM_DATA
    .import REG_OAM_DMA
    .import REG_SCROLL
    .import REG_ADDR
    .import REG_DATA

    ; Buffer addresses
    .import nmt_buffer
    .import palette_buffer
    .import oam_buffer

    ; Zeropage variables used by PPU routines
    .importzp nmi_lock
    .importzp nmi_count
    .importzp nmi_ready
    .importzp nmt_buffer_len

    ; X and Y scroll amounts for next frame
    .importzp scroll_x, scroll_y
    ; Scrolling nametable select (lowest two bits of REG_CTRL) 
    .importzp scroll_nmt
    ; REG_MASK value to write on next frame
	.importzp mask
    ; REG_CTRL value for next frame (will be OR'd with lower two bits of
    ; scroll_mnt)
    .importzp ctrl

    ; PPU Routines
    .import reset
    .import nmi_buffered
    .import update
    .import skip
    .import render_off
    .import address_tile
    .import update_tile_at_xy
    .import update_byte_at_xy
    .import update_next_byte
    ; TODO
    ; .import update_next_bytes_null_terminated
    .import clear_background

    ; An entry in the sprite object attribute memory
    .struct OAMEntry
        y_coord		.byte
        tile_index	.byte
        attributes	.byte
        x_coord		.byte
    .endstruct

.endscope ; PPU

.macro PPU_sta_oam_byte oam_entry, byteno
    sta PPU::oam_buffer + (oam_entry * .sizeof(PPU::OAMEntry)) + byteno
.endmacro

.macro PPU_stx_oam_byte oam_entry, byteno
    stx PPU::oam_buffer + (oam_entry * .sizeof(PPU::OAMEntry)) + byteno
.endmacro

.macro PPU_sty_oam_byte oam_entry, byteno
    sty PPU::oam_buffer + (oam_entry * .sizeof(PPU::OAMEntry)) + byteno
.endmacro

; example:
;   ; sprite 2 gets x=234, y=124, tile number = 3, attributes = 0
;   PPU_update_oam_entry 2, #234, #3, #0, #124
; oam_entry must be a bare numeral
; remaining arguments may be immediate mode or addresses, including indirect
; Clobbers X and A.
.macro PPU_update_known_oam_entry oam_entry, ypos, tileno, attributes, xpos
    ldx #(oam_entry * .sizeof(PPU::OAMEntry))

	; load and store our four arguments
	lda ypos
    sta PPU::oam_buffer, X
	inx
	lda tileno
    sta PPU::oam_buffer, X
	inx
	lda attributes
    sta PPU::oam_buffer, X
	inx
	lda xpos
    sta PPU::oam_buffer, X
.endmacro

; call with:
;	A = index of oam_entry to update
; Clobbers X and A.
.macro PPU_update_oam_entry ypos, tileno, attributes, xpos
	; A = A * 4
	asl
	asl
	; X = A
	tax
	; load and store our four arguments
	lda ypos
	sta oam_buffer, X
	inx
	lda tileno
	sta oam_buffer, X
	inx
	lda attributes
	sta oam_buffer, X
	inx
	lda xpos
	sta oam_buffer, X
.endmacro

.endif ; .ifndef PPU_included
