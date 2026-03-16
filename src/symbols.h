#ifndef SYMBOLS_H
#define SYMBOLS_H

#define MAX_SYMBOLS 256
#define MAX_LABEL   64

typedef struct {
    char         name[MAX_LABEL];
    unsigned int address;
} Symbol;

typedef struct {
    Symbol entries[MAX_SYMBOLS];
    int    count;
} SymbolTable;

void         symbols_init  (SymbolTable *tbl);
/* Returns 0 on success, -1 if duplicate label. */
int          symbols_add   (SymbolTable *tbl, const char *name, unsigned int address);
/* Returns address, or -1 if not found. */
int          symbols_find  (const SymbolTable *tbl, const char *name);

#endif /* SYMBOLS_H */