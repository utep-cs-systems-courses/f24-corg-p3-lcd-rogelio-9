.arch msp430g2553
    .p2align 1,0
    .text

    .global specialScoreInc    ; make function visible to C
    .global callScoreSound     ; second function we'll call

specialScoreInc:
    add #2, r12               ; add 2 to score, r12 holds the first parameter
    push r12                  ; save score value
    call #callScoreSound      ; call the sound function
    pop r12                   ; restore score value
    ret                       ; return to C code
