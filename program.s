.text
.globl main

main:
    addi sp, sp, -192
    mv fp, sp

    li t0, 1
    sw t0, -8(fp)
    li t0, 2
    sw t0, -12(fp)
    li t0, 3
    sw t0, -16(fp)
    li t0, 4
    sw t0, -20(fp)
    lw t0, -8(fp)
    sw t0, 4(fp)
    lw t0, -12(fp)
    lw t1, 4(fp)
    slt t0, t1, t0
    beqz t0, .L0
    lw t0, -8(fp)
    sw t0, 4(fp)
    lw t0, -12(fp)
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, 4(fp)
    lw t0, -20(fp)
    sw t0, 8(fp)
    lw t0, -16(fp)
    lw t1, 8(fp)
    sub t0, t1, t0
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, 4(fp)
    li t0, 1
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, 4(fp)
    lw t0, -16(fp)
    sw t0, 8(fp)
    lw t0, -8(fp)
    lw t1, 8(fp)
    sub t0, t1, t0
    sw t0, 8(fp)
    lw t0, -20(fp)
    sw t0, 12(fp)
    lw t0, -12(fp)
    lw t1, 12(fp)
    sub t0, t1, t0
    lw t1, 8(fp)
    add t0, t1, t0
    lw t1, 4(fp)
    sub t0, t1, t0
    sw t0, -24(fp)
.L0:

    li a7, 93
    ecall
