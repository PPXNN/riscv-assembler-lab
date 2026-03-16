CC     = gcc
CFLAGS = -Wall -Wextra -Isrc

SRC = src/main.c src/assembler.c src/parser.c \
      src/registers.c src/instr_table.c \
      src/symbols.c src/encode.c

# ── Main assembler ───────────────────────────────────────────────────────────
assembler:
	$(CC) $(CFLAGS) $(SRC) -o assembler

# ── Unit tests ───────────────────────────────────────────────────────────────
test_registers:
	$(CC) $(CFLAGS) tests/test_registers.c src/registers.c -o test_registers

test_parser:
	$(CC) $(CFLAGS) tests/test_parser.c src/parser.c -o test_parser

test_instr_table:
	$(CC) $(CFLAGS) tests/test_instr_table.c src/instr_table.c -o test_instr_table

test_symbols:
	$(CC) $(CFLAGS) tests/test_symbols.c src/symbols.c -o test_symbols

test_encode:
	$(CC) $(CFLAGS) tests/test_encode.c src/encode.c -o test_encode

test_assembler:
	$(CC) $(CFLAGS) tests/test_assembler.c \
	      src/assembler.c src/parser.c src/registers.c \
	      src/instr_table.c src/symbols.c src/encode.c \
	      -o test_assembler

# ── Run all tests ────────────────────────────────────────────────────────────
tests: test_registers test_parser test_instr_table test_symbols test_encode test_assembler
	@echo "=== Running all unit tests ==="
	@./test_registers
	@./test_parser
	@./test_instr_table
	@./test_symbols
	@./test_encode
	@./test_assembler
	@echo "=== All tests done ==="

# ── Run assembler on example files ───────────────────────────────────────────
examples: assembler
	./assembler examples/test_case_1.s examples/test_case_1.hex
	./assembler examples/test_case_2.s examples/test_case_2.hex
	./assembler examples/test_case_3.s examples/test_case_3.hex

# ── Clean ─────────────────────────────────────────────────────────────────────
clean:
	rm -f assembler test_registers test_parser test_instr_table \
	      test_symbols test_encode test_assembler \
	      examples/*.hex /tmp/test_asm_*.s /tmp/test_asm_*.hex

.PHONY: assembler tests examples clean	