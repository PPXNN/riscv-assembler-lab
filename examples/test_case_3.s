# test_case_3.s
# 120+ lines: comprehensive stress test.
# Implements: array fill, bubble sort, checksum, fibonacci.
# Exercises every supported instruction at least once.

# ============================================================
# Register convention (software):
#   x1  (ra)  - return address
#   x2  (sp)  - stack pointer (unused here, but initialised)
#   x8  (s0)  - saved / frame pointer
#   x10 (a0)  - argument / return value
#   x5-x7     - temporaries
# ============================================================

        # --- Initialise base pointers ---
        addi  x2,  x0,  0x400  # sp = 0x400
        addi  x8,  x0,  0x100  # array base = 0x100
        addi  x9,  x0,  8      # array length = 8

        # ========================================================
        # SECTION 1: Fill array with values 8,7,6,5,4,3,2,1
        # ========================================================
        addi  x10, x0,  8      # value = 8
        addi  x11, x0,  0      # index = 0
fill_loop:
        slli  x12, x11, 2      # byte offset = index * 4
        add   x13, x8,  x12    # address = base + offset
        sw    x10, 0(x13)      # mem[address] = value
        addi  x10, x10, -1     # value--
        addi  x11, x11, 1      # index++
        blt   x11, x9,  fill_loop

        # ========================================================
        # SECTION 2: Bubble sort (ascending)
        # ========================================================
        addi  x14, x0,  1      # outer = 1  (outer loop flag)
outer_loop:
        beq   x14, x0,  sort_done
        addi  x14, x0,  0      # assume sorted
        addi  x11, x0,  0      # i = 0
        addi  x15, x9,  -1     # limit = n-1
inner_loop:
        bge   x11, x15, inner_done
        slli  x12, x11, 2
        add   x13, x8,  x12    # &a[i]
        lw    x5,  0(x13)      # a[i]
        addi  x16, x13, 4      # &a[i+1]
        lw    x6,  0(x16)      # a[i+1]
        ble_skip:
        bge   x5,  x6,  swap   # if a[i] > a[i+1]: swap
        jal   x0,  no_swap
swap:
        sw    x6,  0(x13)      # a[i]   = a[i+1]
        sw    x5,  0(x16)      # a[i+1] = a[i]
        addi  x14, x0,  1      # swapped = true
no_swap:
        addi  x11, x11, 1      # i++
        jal   x0,  inner_loop
inner_done:
        jal   x0,  outer_loop
sort_done:

        # ========================================================
        # SECTION 3: Compute array checksum (XOR of all elements)
        # ========================================================
        addi  x17, x0,  0      # checksum = 0
        addi  x11, x0,  0      # i = 0
xor_loop:
        bge   x11, x9,  xor_done
        slli  x12, x11, 2
        add   x13, x8,  x12
        lw    x5,  0(x13)
        xor   x17, x17, x5     # checksum ^= a[i]
        addi  x11, x11, 1
        jal   x0,  xor_loop
xor_done:
        sw    x17, 0(x2)       # store checksum at sp

        # ========================================================
        # SECTION 4: Fibonacci sequence  fib(0..7)
        # Stores results at 0x200
        # ========================================================
        addi  x18, x0,  0x200  # fib base address
        addi  x19, x0,  0      # fib(0) = 0
        addi  x20, x0,  1      # fib(1) = 1
        sw    x19, 0(x18)
        sw    x20, 4(x18)
        addi  x11, x0,  2      # index = 2
        addi  x21, x0,  8      # limit = 8
fib_loop:
        bge   x11, x21, fib_done
        add   x22, x19, x20    # fib(n) = fib(n-1) + fib(n-2)
        slli  x12, x11, 2
        add   x13, x18, x12
        sw    x22, 0(x13)
        addi  x19, x20, 0      # slide window
        addi  x20, x22, 0
        addi  x11, x11, 1
        jal   x0,  fib_loop
fib_done:

        # ========================================================
        # SECTION 5: Bit-manipulation smoke tests
        # ========================================================
        lw    x5,  0(x18)      # fib[0] = 0
        lw    x6,  4(x18)      # fib[1] = 1
        lw    x7,  8(x18)      # fib[2] = 1

        and   x23, x6,  x7
        or    x23, x23, x5
        xor   x24, x6,  x7
        sll   x25, x6,  x7     # 1 << 1 = 2
        srl   x26, x25, x6     # 2 >> 1 = 1
        sra   x27, x25, x6     # 2 >>a 1 = 1
        slt   x28, x5,  x6     # 0 < 1 → 1
        sltu  x29, x5,  x6     # 0 <u 1 → 1

        # Immediate variants
        addi  x23, x5,  0xFF
        andi  x24, x23, 0x0F
        ori   x24, x24, 0xF0
        xori  x24, x24, 0xFF
        slti  x28, x5,  1      # 0 < 1 → 1
        sltiu x29, x5,  1      # 0 <u 1 → 1
        slli  x25, x6,  4      # 1 << 4 = 16
        srli  x26, x25, 2      # 16 >> 2 = 4
        srai  x27, x25, 2      # 16 >>a 2 = 4

        # Byte/half load-store
        sb    x6,  0(x2)
        sh    x6,  2(x2)
        lb    x30, 0(x2)
        lbu   x30, 0(x2)
        lh    x30, 2(x2)
        lhu   x30, 2(x2)

        # ========================================================
        # SECTION 6: JAL / JALR round-trip
        # ========================================================
        jal   x1,  sub_routine
        jal   x0,  halt

sub_routine:
        addi  x10, x10, 1      # a0++
        jalr  x0,  x1,  0      # return

        # ========================================================
        # SECTION 7: All branch conditions once more
        # ========================================================
        addi  x3,  x0,  5
        addi  x4,  x0,  5
        beq   x3,  x4,  b_ne
b_ne:   addi  x3,  x3,  1
        bne   x3,  x4,  b_lt
b_lt:   addi  x4,  x4,  10
        blt   x3,  x4,  b_bge
b_bge:  bge   x4,  x3,  b_bltu
b_bltu: bltu  x3,  x4,  b_bgeu
b_bgeu: bgeu  x4,  x3,  halt

halt:
        jal   x0,  halt