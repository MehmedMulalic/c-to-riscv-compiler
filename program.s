.data
.FC0: .float 0.5

.text
.globl main

main:
    addi sp, sp, -208
    mv fp, sp

    li t0, 4
    sw t0, -8(fp)
    li t0, 6
    sw t0, -12(fp)
    li t0, 3
    sw t0, -16(fp)
    lw t0, -8(fp)
    sw t0, 4(fp)
    lui t0, %hi(.FC0)
    flw ft0, %lo(.FC0)(t0)
    lw t1, 4(fp)
    fcvt.s.w ft1, t1
    fadd.s ft0, ft1, ft0
    fsw ft0, -44(fp)

    li a7, 93
    ecall
