# RISC-V Assembler (Lab 8-4)

A two-pass assembler for a subset of **RV32I**, written in C as part of CS219223 Computer System Laboratory.

---

## Supported Instructions

| Category           | Instructions |
|--------------------|-------------|
| R-type arithmetic  | `add sub and or xor sll srl sra slt sltu` |
| I-type arithmetic  | `addi andi ori xori slli srli srai slti sltiu` |
| Loads              | `lb lbu lh lhu lw` |
| Stores             | `sb sh sw` |
| Branches           | `beq bne blt bge bltu bgeu` |
| Jumps              | `jal jalr` |

---

## Repository Structure

```
riscv-assembler-lab/
├── Makefile
├── README.md
├── src/
│   ├── main.c          # Entry point
│   ├── assembler.c/h   # Two-pass pipeline
│   ├── parser.c/h      # Line tokeniser
│   ├── registers.c/h   # Register name → number
│   ├── instr_table.c/h # Instruction specification table
│   ├── symbols.c/h     # Label → address table
│   └── encode.c/h      # Bit-field encoders (R/I/S/B/U/J)
├── tests/
│   ├── test_registers.c
│   ├── test_parser.c
│   ├── test_instr_table.c
│   ├── test_symbols.c
│   ├── test_encode.c
│   └── test_assembler.c
└── examples/
    ├── test_case_1.s   # ~20 lines
    ├── test_case_2.s   # ~50 lines
    └── test_case_3.s   # ~120 lines
```

---

## Build & Run

### Build the assembler
```bash
make assembler
```

### Assemble a file
```bash
./assembler examples/test_case_1.s            # produces test_case_1.hex
./assembler examples/test_case_1.s output.hex # custom output name
```

Output is one 32-bit word per line in hex, e.g.:
```
0x00500093
0x00A00113
0x003100B3
```

---

## Running Unit Tests

```bash
# Build and run all unit tests at once
make tests

# Or run individually
make test_registers && ./test_registers
make test_parser    && ./test_parser
make test_encode    && ./test_encode
```

---

## Running Example Programs

```bash
make examples
# produces examples/test_case_1.hex, test_case_2.hex, test_case_3.hex
```

---

## Two-Pass Algorithm

**Pass 1** — scan every line, record label → PC mappings in the symbol table.  
**Pass 2** — re-scan, encode each instruction; branches and jumps use the symbol table to compute PC-relative offsets.

```
offset = label_address − current_PC
```

---

## Assembly Syntax

```asm
# This is a comment

label:  mnemonic  rd, rs1, rs2       # R-type
        addi      x1, x0, 42         # I-type
        lw        x1, 8(x2)          # load
        sw        x3, 8(x2)          # store
        beq       x1, x2, label      # branch
        jal       x1, label          # jump-and-link
        jalr      x0, x1, 0          # jump-and-link-register
```

Register names may be `x0`–`x31` or ABI aliases (`zero`, `ra`, `sp`, `a0`, …).

---

## Verification

Assembled `.hex` output was verified against the **Venus RISC-V simulator** for all three test cases. See `examples/` for `.hex` files and the test report.