.text
.globl main

main:
    addi sp, sp, -192
    mv fp, sp

    li t0, 1
    sw t0, -12(fp)
    li t0, 2
    sw t0, -16(fp)
    li t0, 3
    sw t0, -20(fp)
    li t0, 4
    sw t0, -24(fp)
    lw t0, -12(fp)
    sw t0, 4(fp)
    lw t0, -16(fp)
    lw t1, 4(fp)
    slt t0, t1, t0
    beqz t0, .L0
    lw t0, -12(fp)
    sw t0, 4(fp)
    lw t0, -16(fp)
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, 4(fp)
    lw t0, -24(fp)
    sw t0, 8(fp)
    lw t0, -20(fp)
    lw t1, 8(fp)
    sub t0, t1, t0
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, 4(fp)
    li t0, 1
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, 4(fp)
    lw t0, -20(fp)
    sw t0, 8(fp)
    lw t0, -12(fp)
    lw t1, 8(fp)
    sub t0, t1, t0
    sw t0, 8(fp)
    lw t0, -24(fp)
    sw t0, 12(fp)
    lw t0, -16(fp)
    lw t1, 12(fp)
    sub t0, t1, t0
    lw t1, 8(fp)
    add t0, t1, t0
    lw t1, 4(fp)
    sub t0, t1, t0
    sw t0, -28(fp)
    j .L1
.L0:
    li t0, 2
    sw t0, -28(fp)
.L1:
    lw t0, -28(fp)
    li a7, 1
    mv a0, t0
    ecall
    li a0, 10
    li a7, 11
    ecall

    li a0, 0

    li a7, 93
    ecall
