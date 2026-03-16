#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

typedef enum {
    FMT_R, FMT_I, FMT_S, FMT_B, FMT_U, FMT_J
} InstrFormat;

typedef struct {
    const char  *mnemonic;
    InstrFormat  fmt;
    unsigned int opcode;   /* 7-bit opcode */
    unsigned int funct3;   /* 3-bit funct3 (0 if unused) */
    unsigned int funct7;   /* 7-bit funct7 (0 if unused) */
} InstrSpec;

/* Returns pointer to matching entry, or NULL if not found. */
const InstrSpec *instr_lookup(const char *mnemonic);

#endif /* INSTR_TABLE_H */