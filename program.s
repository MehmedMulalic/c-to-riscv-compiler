.text
.globl main

main:
    addi sp, sp, -16
    lw t0, -8(fp)
    mv t1, t0
    li t0, 5
    mv t1, t0
    li t0, 3
    add t0, t1, t0
    sw t0, -8(fp)
    mv t1, t0
    lw t0, -12(fp)
    mv t1, t0
    lw t0, -8(fp)
    mv t1, t0
    li t0, 10
    add t0, t1, t0
    sw t0, -12(fp)

    li a7, 93
    ecall
