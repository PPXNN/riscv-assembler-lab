# test_case_2.s
# ~50 lines: broader coverage including all branch types, byte/half loads/stores,
# shift variants, jalr, sra, forward and backward branches.

        # --- Initialise registers ---
        addi  x1,  x0, 10      # x1  = 10
        addi  x2,  x0, 20      # x2  = 20
        addi  x3,  x0, -5      # x3  = -5  (negative)
        addi  x4,  x0, 200     # x4  = base address 200
        addi  x5,  x0, 0       # x5  = accumulator

        # --- Store byte / half / word ---
        sb    x1,  0(x4)       # mem[200] = lo-byte of x1
        sh    x2,  2(x4)       # mem[202] = lo-half of x2
        sw    x1,  4(x4)       # mem[204] = x1

        # --- Load byte / half / word ---
        lb    x6,  0(x4)       # sign-extended byte
        lbu   x7,  0(x4)       # zero-extended byte
        lh    x8,  2(x4)       # sign-extended half
        lhu   x9,  2(x4)       # zero-extended half
        lw    x10, 4(x4)       # full word

        # --- Arithmetic ---
        add   x11, x1,  x2     # 30
        sub   x12, x2,  x1     # 10
        and   x13, x1,  x2     # 10 & 20 = 0
        or    x14, x1,  x2     # 10 | 20 = 30
        xor   x15, x1,  x2     # 10 ^ 20 = 30

        # --- Shifts ---
        sll   x16, x1,  x2     # x1 << (x2 & 31)
        srl   x17, x2,  x1     # logical right shift
        sra   x18, x3,  x1     # arithmetic right (sign-preserving)
        slli  x19, x1,  3      # 10 << 3 = 80
        srli  x20, x2,  1      # 20 >> 1 = 10
        srai  x21, x3,  1      # -5 >> 1 = -3 (arithmetic)

        # --- Immediate arithmetic ---
        addi  x22, x1,  100    # 110
        andi  x23, x2,  0x1F   # 20 & 31 = 20
        ori   x24, x1,  0x80   # 10 | 128 = 138
        xori  x25, x2,  0xFF   # 20 ^ 255 = 235
        slti  x26, x3,  0      # -5 < 0 → 1
        sltiu x27, x1,  100    # 10 <u 100 → 1

        # --- Set-less-than ---
        slt   x28, x3,  x1     # -5 < 10 → 1
        sltu  x29, x1,  x2     # 10 <u 20 → 1

        # --- All branch types ---
        beq   x1,  x1,  eq_ok  # always taken
eq_ok:
        bne   x1,  x2,  ne_ok  # taken (10 != 20)
ne_ok:
        blt   x3,  x1,  lt_ok  # taken (-5 < 10)
lt_ok:
        bge   x1,  x3,  ge_ok  # taken (10 >= -5)
ge_ok:
        bltu  x1,  x2,  ltu_ok # taken (10 <u 20)
ltu_ok:
        bgeu  x2,  x1,  geu_ok # taken (20 >=u 10)
geu_ok:

        # --- JAL / JALR ---
        jal   x30, after_jal   # jump and link
after_jal:
        addi  x5,  x5,  1      # x5++
        jalr  x0,  x30, 0      # return (jump to ra)

        # --- Backward branch test ---
        addi  x31, x0,  3      # counter = 3
count_down:
        addi  x31, x31, -1     # counter--
        bne   x31, x0,  count_down  # loop while != 0

halt:
        jal   x0,  halt        # halt