.ifndef Random_included
Random_included = 1 ;;
.linecont +

.include "math_macros.inc"

.scope Random
    .globalzp crc_lo
    .globalzp crc_hi

    ;; .. segment:: CODE

    .macro Random_seed_crc16 val16
        .if (.match (.left (1, {val16}), #))
            lda #<(.right (.tcount ({val16})-1, {val16}))
        .else
            lda val16 + 0
        .endif
        sta Random::crc_lo

        .if (.match (.left (1, {val16}), #))
            lda #>(.right (.tcount ({val16})-1, {val16}))
        .else
            lda val16 + 1
        .endif
        sta Random::crc_hi
    .endmacro

    .global random_crc16    ;; .. subroutine:: random_crc16
        ;;
        ;; Places the next two random bytes in A and X. Implemented via
        ;; :subr:`update_crc16` by updating the CRC-16 for one new ``$00`` byte
        ;; of "data." The new CRC-16 is the next 16-bit pseudorandom value.
        ;;
        ;; :callwith:           ``jsr``
        ;; :output register A:  "High" byte of the next random 16-bit value.
        ;;                      Also available as :label:`crc_hi`.
        ;; :output register X:  "Low" byte of the next random 16-bit value.
        ;;                      Also available as :label:`crc_lo`.
        ;; :clobbers:           A, X.

    .global update_crc16

    ; TODO: 16-bit version
    .macro rand8_between min, max
        ;;
        ;; Gets a pseudorandom byte between constants min and max. Sets A = a
        ;; random byte in [min, max), using random_crc16 followed by a mask,
        ;; modulus, and addition.
        ;;
        ;; :output register A:  The result.
        ;; :clobbers:           A, X. X will have a psuedorandom value,
        ;;                      unconstrained.

        .assert max > min, error, \
            .sprintf("rand8_between: max must be > min, got: min = %d, max = %d", min, max)
        .local range
        .local mask

        jsr Random::random_crc16        ; A = random byte
        range = max - min
        ; Speed up the modulus computation by masking out bits unnecessary for
        ; representing numbers >= range. mask = N-1 where N is the smallest
        ; power of 2 >= range. E.g., if range == 4, we mask with %111.
        ; This probably causes us to lose some entropy. (Algo from Hacker's
        ; Delight, also found in MAGIC 
        ; (http://aggregate.org/MAGIC/#Next%20Largest%20Power%20of%202) and
        ; in Bit Twiddling Hacks
        ; (http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2).
        mask .set (range - 1)
        mask .set mask | (mask >> 1)
        mask .set mask | (mask >> 2)
        mask .set mask | (mask >> 4)
        and #mask

        mathmac_mod8 #range
        .if min > 0
            clc
            adc #min
        .endif
        ; A = random byte in [min, max).
    .endmacro

.endscope

.endif ; Random_included
