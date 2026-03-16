#include "../src/parser.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static int pass = 0, fail = 0;

static void check_str(const char *got, const char *expected, const char *ctx) {
    if (strcmp(got, expected) == 0) { pass++; }
    else { fprintf(stderr, "FAIL [%s]: got '%s' expected '%s'\n", ctx, got, expected); fail++; }
}
static void check_int(int got, int expected, const char *ctx) {
    if (got == expected) { pass++; }
    else { fprintf(stderr, "FAIL [%s]: got %d expected %d\n", ctx, got, expected); fail++; }
}

int main(void) {
    ParsedLine pl;

    /* Basic R-type with label */
    parse_line("loop: add x1, x2, x3", &pl);
    check_str(pl.label, "loop", "label");
    check_str(pl.mnemonic, "add", "mnemonic");
    check_str(pl.operands[0], "x1", "op0");
    check_str(pl.operands[1], "x2", "op1");
    check_str(pl.operands[2], "x3", "op2");
    check_int(pl.num_operands, 3, "num_operands");

    /* I-type arithmetic */
    parse_line("addi x5, x0, 42", &pl);
    check_str(pl.label, "", "no label");
    check_str(pl.mnemonic, "addi", "mnemonic addi");
    check_int(pl.num_operands, 3, "addi nops");
    check_str(pl.operands[2], "42", "imm");

    /* Load: lw x1, 8(x2) */
    parse_line("lw x1, 8(x2)", &pl);
    check_str(pl.mnemonic, "lw", "lw mnemonic");
    check_str(pl.operands[0], "x1", "lw rd");
    check_str(pl.operands[1], "8",  "lw imm");
    check_str(pl.operands[2], "x2", "lw rs1");

    /* Comment-only line */
    int r = parse_line("# this is a comment", &pl);
    check_int(r, 0, "comment line");

    /* Blank line */
    r = parse_line("   ", &pl);
    check_int(r, 0, "blank line");

    /* Branch */
    parse_line("beq x1, x2, done", &pl);
    check_str(pl.mnemonic, "beq", "beq mnemonic");
    check_str(pl.operands[2], "done", "branch label");

    printf("parser tests: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}