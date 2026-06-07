.data
.FC0: .float 2.5
.FC1: .float 4.0
.FC2: .float 1.5
.FC3: .float 2.0
.FC4: .float 1.0
.FC5: .float 0.5

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
    lw t0, -12(fp)
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, -20(fp)
    lw t0, -20(fp)
    sw t0, 4(fp)
    lw t0, -16(fp)
    lw t1, 4(fp)
    mul t0, t1, t0
    sw t0, -20(fp)
    lw t0, -20(fp)
    sw t0, 4(fp)
    li t0, 3
    lw t1, 4(fp)
    div t0, t1, t0
    sw t0, -20(fp)
    lw t0, -20(fp)
    sw t0, 4(fp)
    li t0, 2
    lw t1, 4(fp)
    sub t0, t1, t0
    sw t0, -20(fp)
    lw t0, -8(fp)
    sw t0, 4(fp)
    lw t0, -12(fp)
    lw t1, 4(fp)
    slt t0, t1, t0
    beqz t0, .L0
    li t0, 1
    sw t0, -20(fp)
    j .L1
.L0:
    li t0, 0
    sw t0, -20(fp)
.L1:
    lw t0, -8(fp)
    sw t0, 4(fp)
    lw t0, -12(fp)
    lw t1, 4(fp)
    slt t0, t0, t1
    beqz t0, .L2
    li t0, 100
    sw t0, -20(fp)
    j .L3
.L2:
    li t0, 200
    sw t0, -20(fp)
.L3:
    lw t0, -8(fp)
    sw t0, 4(fp)
    lw t0, -12(fp)
    lw t1, 4(fp)
    slt t0, t1, t0
    beqz t0, .L4
    lw t0, -16(fp)
    sw t0, 4(fp)
    lw t0, -12(fp)
    lw t1, 4(fp)
    slt t0, t1, t0
    beqz t0, .L6
    li t0, 7
    sw t0, -20(fp)
    j .L7
.L6:
    li t0, 9
    sw t0, -20(fp)
.L7:
    j .L5
.L4:
    li t0, 11
    sw t0, -20(fp)
.L5:
    li t0, 0
    sw t0, -24(fp)
    li t0, 0
    sw t0, -28(fp)
.L8:
    lw t0, -24(fp)
    sw t0, 4(fp)
    li t0, 4
    lw t1, 4(fp)
    slt t0, t1, t0
    beqz t0, .L9
    lw t0, -28(fp)
    sw t0, 4(fp)
    lw t0, -24(fp)
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, -28(fp)
    lw t0, -24(fp)
    sw t0, 4(fp)
    li t0, 1
    lw t1, 4(fp)
    add t0, t1, t0
    sw t0, -24(fp)
    j .L8
.L9:
    lui t0, %hi(.FC0)
    flw ft0, %lo(.FC0)(t0)
    fsw ft0, -32(fp)
    lui t0, %hi(.FC1)
    flw ft0, %lo(.FC1)(t0)
    fsw ft0, -36(fp)
    lui t0, %hi(.FC2)
    flw ft0, %lo(.FC2)(t0)
    fsw ft0, -40(fp)
    flw ft0, -32(fp)
    fsw ft0, 4(fp)
    flw ft0, -36(fp)
    flw ft1, 4(fp)
    fadd.s ft0, ft1, ft0
    fsw ft0, -44(fp)
    flw ft0, -44(fp)
    fsw ft0, 4(fp)
    flw ft0, -40(fp)
    flw ft1, 4(fp)
    fsub.s ft0, ft1, ft0
    fsw ft0, -44(fp)
    flw ft0, -44(fp)
    fsw ft0, 4(fp)
    lui t0, %hi(.FC3)
    flw ft0, %lo(.FC3)(t0)
    flw ft1, 4(fp)
    fmul.s ft0, ft1, ft0
    fsw ft0, -44(fp)
    flw ft0, -44(fp)
    fsw ft0, 4(fp)
    lui t0, %hi(.FC1)
    flw ft0, %lo(.FC1)(t0)
    flw ft1, 4(fp)
    fdiv.s ft0, ft1, ft0
    fsw ft0, -44(fp)
    flw ft0, -44(fp)
    fsw ft0, 4(fp)
    lui t0, %hi(.FC3)
    flw ft0, %lo(.FC3)(t0)
    flw ft1, 4(fp)
    flt.s t0, ft0, ft1
    beqz t0, .L10
    flw ft0, -44(fp)
    fsw ft0, 4(fp)
    lui t0, %hi(.FC4)
    flw ft0, %lo(.FC4)(t0)
    flw ft1, 4(fp)
    fadd.s ft0, ft1, ft0
    fsw ft0, -44(fp)
    j .L11
.L10:
    flw ft0, -44(fp)
    fsw ft0, 4(fp)
    lui t0, %hi(.FC4)
    flw ft0, %lo(.FC4)(t0)
    flw ft1, 4(fp)
    fsub.s ft0, ft1, ft0
    fsw ft0, -44(fp)
.L11:
    lw t0, -8(fp)
    sw t0, 4(fp)
    lui t0, %hi(.FC5)
    flw ft0, %lo(.FC5)(t0)
    lw t1, 4(fp)
    fcvt.s.w ft1, t1
    fadd.s ft0, ft1, ft0
    fsw ft0, -44(fp)
    lui t0, %hi(.FC5)
    flw ft0, %lo(.FC5)(t0)
    fsw ft0, 4(fp)
    lw t0, -8(fp)
    flw ft1, 4(fp)
    fcvt.s.w ft0, t0
    fadd.s ft0, ft1, ft0
    fsw ft0, -44(fp)

    li a7, 93
    ecall
