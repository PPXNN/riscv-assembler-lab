#include "symbols.h"
#include <string.h>
#include <stdio.h>

void symbols_init(SymbolTable *tbl) {
    tbl->count = 0;
}

int symbols_add(SymbolTable *tbl, const char *name, unsigned int address) {
    /* Detect duplicate */
    if (symbols_find(tbl, name) >= 0) {
        fprintf(stderr, "Error: duplicate label '%s'\n", name);
        return -1;
    }
    if (tbl->count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: symbol table full\n");
        return -1;
    }
    strncpy(tbl->entries[tbl->count].name, name, MAX_LABEL-1);
    tbl->entries[tbl->count].address = address;
    tbl->count++;
    return 0;
}

int symbols_find(const SymbolTable *tbl, const char *name) {
    for (int i = 0; i < tbl->count; i++)
        if (strcmp(tbl->entries[i].name, name) == 0)
            return (int)tbl->entries[i].address;
    return -1;
}