#include "../src/instr_table.h"
#include <stdio.h>
#include <assert.h>

static int pass = 0, fail = 0;
#define CHECK(cond, msg) \
    do { if(cond){pass++;}else{fprintf(stderr,"FAIL: %s\n",msg);fail++;} } while(0)

int main(void) {
    const InstrSpec *s;

    s = instr_lookup("add");
    CHECK(s != NULL,        "add found");
    CHECK(s->fmt == FMT_R,  "add fmt R");
    CHECK(s->opcode == 0x33,"add opcode");
    CHECK(s->funct3 == 0,   "add funct3");
    CHECK(s->funct7 == 0,   "add funct7");

    s = instr_lookup("sub");
    CHECK(s && s->funct7 == 0x20, "sub funct7");

    s = instr_lookup("addi");
    CHECK(s && s->fmt == FMT_I && s->opcode == 0x13, "addi");

    s = instr_lookup("lw");
    CHECK(s && s->fmt == FMT_I && s->opcode == 0x03 && s->funct3 == 2, "lw");

    s = instr_lookup("sw");
    CHECK(s && s->fmt == FMT_S && s->opcode == 0x23 && s->funct3 == 2, "sw");

    s = instr_lookup("beq");
    CHECK(s && s->fmt == FMT_B && s->opcode == 0x63 && s->funct3 == 0, "beq");

    s = instr_lookup("jal");
    CHECK(s && s->fmt == FMT_J && s->opcode == 0x6F, "jal");

    s = instr_lookup("jalr");
    CHECK(s && s->fmt == FMT_I && s->opcode == 0x67, "jalr");

    s = instr_lookup("srai");
    CHECK(s && s->funct7 == 0x20, "srai funct7");

    s = instr_lookup("nonexistent");
    CHECK(s == NULL, "unknown returns NULL");

    printf("instr_table tests: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}