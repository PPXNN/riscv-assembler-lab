#include "assembler.h"
#include "parser.h"
#include "registers.h"
#include "instr_table.h"
#include "symbols.h"
#include "encode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 2048
#define LINE_BUF  256

/* -------------------------------------------------------------------------
   Utility: parse an integer literal or look it up in the symbol table.
   Returns value on success, sets *ok=0 on error.
   ------------------------------------------------------------------------- */
static int resolve_imm(const char *tok, const SymbolTable *sym, int pc, int *ok) {
    *ok = 1;
    /* Try numeric first */
    char *end;
    long v = strtol(tok, &end, 0);
    if (*tok != '\0' && *end == '\0') return (int)v;

    /* Try symbol table */
    int addr = symbols_find(sym, tok);
    if (addr >= 0) return addr - pc;   /* offset from current PC */

    fprintf(stderr, "Error: cannot resolve '%s'\n", tok);
    *ok = 0;
    return 0;
}

/* -------------------------------------------------------------------------
   Pass 1: read every line, record labels, count instruction addresses.
   Only real instructions advance the PC (labels do not).
   ------------------------------------------------------------------------- */
static int pass1(FILE *fp, SymbolTable *sym) {
    char line[LINE_BUF];
    unsigned int pc = 0;

    while (fgets(line, sizeof(line), fp)) {
        ParsedLine pl;
        int has_instr = parse_line(line, &pl);

        /* Record label at current PC */
        if (pl.label[0] != '\0') {
            if (symbols_add(sym, pl.label, pc) != 0)
                return -1;
        }

        /* Advance PC only if there is an instruction */
        if (has_instr && pl.mnemonic[0] != '\0')
            pc += 4;
    }
    return 0;
}

/* -------------------------------------------------------------------------
   Pass 2: encode every instruction and write to output file.
   ------------------------------------------------------------------------- */
static int pass2(FILE *fp, FILE *out, const SymbolTable *sym) {
    char line[LINE_BUF];
    unsigned int pc = 0;

    while (fgets(line, sizeof(line), fp)) {
        ParsedLine pl;
        int has_instr = parse_line(line, &pl);
        if (!has_instr || pl.mnemonic[0] == '\0') continue;

        const InstrSpec *spec = instr_lookup(pl.mnemonic);
        if (!spec) {
            fprintf(stderr, "Error at PC=0x%x: unknown mnemonic '%s'\n",
                    pc, pl.mnemonic);
            return -1;
        }

        uint32_t instr = 0;
        int ok = 1;

        switch (spec->fmt) {

        case FMT_R: {
            /* add rd, rs1, rs2 */
            int rd  = parse_register(pl.operands[0]);
            int rs1 = parse_register(pl.operands[1]);
            int rs2 = parse_register(pl.operands[2]);
            if (rd<0||rs1<0||rs2<0) { ok=0; break; }
            instr = encode_r(spec->opcode, spec->funct3, spec->funct7,
                             (unsigned)rd, (unsigned)rs1, (unsigned)rs2);
            break;
        }

        case FMT_I: {
            if (spec->opcode == 0x03) {
                /* Load: lw rd, imm(rs1) → operands: rd, imm, rs1  (after parser splits 'imm(rs1)') */
                int rd  = parse_register(pl.operands[0]);
                int rs1 = parse_register(pl.operands[2]);
                int imm = (int)strtol(pl.operands[1], NULL, 0);
                if (rd<0||rs1<0) { ok=0; break; }
                instr = encode_i(spec->opcode, spec->funct3,
                                 (unsigned)rd, (unsigned)rs1, imm);
            } else if (spec->opcode == 0x67) {
                /* jalr rd, rs1, imm  OR  jalr rd, imm(rs1) */
                int rd, rs1, imm;
                if (pl.num_operands == 3 &&
                    parse_register(pl.operands[1]) >= 0) {
                    /* jalr rd, rs1, imm */
                    rd  = parse_register(pl.operands[0]);
                    rs1 = parse_register(pl.operands[1]);
                    imm = (int)strtol(pl.operands[2], NULL, 0);
                } else {
                    /* jalr rd, imm(rs1) */
                    rd  = parse_register(pl.operands[0]);
                    imm = (int)strtol(pl.operands[1], NULL, 0);
                    rs1 = parse_register(pl.operands[2]);
                }
                if (rd<0||rs1<0) { ok=0; break; }
                instr = encode_i(spec->opcode, spec->funct3,
                                 (unsigned)rd, (unsigned)rs1, imm);
            } else {
                /* Immediate arithmetic: addi rd, rs1, imm */
                int rd  = parse_register(pl.operands[0]);
                int rs1 = parse_register(pl.operands[1]);
                int imm;
                if (strcmp(pl.mnemonic,"slli")==0 ||
                    strcmp(pl.mnemonic,"srli")==0 ||
                    strcmp(pl.mnemonic,"srai")==0) {
                    /* shift amount in bits [4:0], funct7 in bits [11:5] */
                    int shamt = (int)strtol(pl.operands[2], NULL, 0) & 0x1F;
                    imm = (int)(((spec->funct7 & 0x7F) << 5) | shamt);
                } else {
                    imm = resolve_imm(pl.operands[2], sym, (int)pc, &ok);
                }
                if (rd<0||rs1<0) { ok=0; break; }
                instr = encode_i(spec->opcode, spec->funct3,
                                 (unsigned)rd, (unsigned)rs1, imm);
            }
            break;
        }

        case FMT_S: {
            /* sw rs2, imm(rs1) → operands: rs2, imm, rs1 */
            int rs2 = parse_register(pl.operands[0]);
            int rs1 = parse_register(pl.operands[2]);
            int imm = (int)strtol(pl.operands[1], NULL, 0);
            if (rs1<0||rs2<0) { ok=0; break; }
            instr = encode_s(spec->opcode, spec->funct3,
                             (unsigned)rs1, (unsigned)rs2, imm);
            break;
        }

        case FMT_B: {
            /* beq rs1, rs2, label */
            int rs1 = parse_register(pl.operands[0]);
            int rs2 = parse_register(pl.operands[1]);
            int imm = resolve_imm(pl.operands[2], sym, (int)pc, &ok);
            if (rs1<0||rs2<0) { ok=0; break; }
            instr = encode_b(spec->opcode, spec->funct3,
                             (unsigned)rs1, (unsigned)rs2, imm);
            break;
        }

        case FMT_U: {
            int rd  = parse_register(pl.operands[0]);
            int imm = resolve_imm(pl.operands[1], sym, (int)pc, &ok);
            if (rd<0) { ok=0; break; }
            instr = encode_u(spec->opcode, (unsigned)rd, imm);
            break;
        }

        case FMT_J: {
            /* jal rd, label */
            int rd  = parse_register(pl.operands[0]);
            int imm = resolve_imm(pl.operands[1], sym, (int)pc, &ok);
            if (rd<0) { ok=0; break; }
            instr = encode_j(spec->opcode, (unsigned)rd, imm);
            break;
        }

        default:
            ok = 0;
            break;
        }

        if (!ok) {
            fprintf(stderr, "Error encoding '%s' at PC=0x%x\n", pl.mnemonic, pc);
            return -1;
        }

        fprintf(out, "0x%08X\n", instr);
        pc += 4;
    }
    return 0;
}

/* -------------------------------------------------------------------------
   Public entry point
   ------------------------------------------------------------------------- */
int assemble_file(const char *input_path, const char *output_path) {
    FILE *fp = fopen(input_path, "r");
    if (!fp) {
        perror(input_path);
        return -1;
    }

    SymbolTable sym;
    symbols_init(&sym);

    /* Pass 1 */
    if (pass1(fp, &sym) != 0) { fclose(fp); return -1; }

    /* Pass 2 */
    rewind(fp);
    FILE *out = fopen(output_path, "w");
    if (!out) { perror(output_path); fclose(fp); return -1; }

    int rc = pass2(fp, out, &sym);
    fclose(fp);
    fclose(out);
    return rc;
}