# test_case_1.s
# ~20 lines: arithmetic, memory, branches, jal
# Computes sum = 1+2+3+4+5 and stores result to memory

        addi  x1, x0, 0        # sum = 0
        addi  x2, x0, 1        # i   = 1
        addi  x3, x0, 5        # limit = 5
        addi  x4, x0, 100      # base address

loop:
        add   x1, x1, x2       # sum += i
        addi  x2, x2, 1        # i++
        slt   x5, x3, x2       # x5 = (limit < i)  → 1 when done
        beq   x5, x0, loop     # if not done, repeat

        sw    x1, 0(x4)        # store sum at mem[100]
        lw    x6, 0(x4)        # reload to verify

        # Bitwise checks
        andi  x7, x1, 0xFF     # mask low byte
        ori   x8, x1, 0x100    # set bit 8
        xori  x9, x1, 0x0F     # flip low nibble
        slli  x10, x1, 2       # sum << 2
        srli  x11, x1, 1       # sum >> 1

        # Unsigned / signed compare
        sltu  x12, x2, x3      # x12 = (i <u limit)
        sub   x13, x1, x6      # should be 0

done:
        jal   x0, done         # infinite loop (halt)