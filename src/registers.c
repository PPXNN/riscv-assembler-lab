#include "registers.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* ABI name table – index = register number */
static const char *abi_names[] = {
    "zero","ra","sp","gp","tp",
    "t0","t1","t2",
    "s0","s1",
    "a0","a1","a2","a3","a4","a5","a6","a7",
    "s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
    "t3","t4","t5","t6"
};

int parse_register(const char *name) {
    if (!name) return -1;

    /* Strip leading whitespace */
    while (isspace((unsigned char)*name)) name++;

    /* xN form */
    if (name[0] == 'x') {
        char *end;
        long n = strtol(name + 1, &end, 10);
        if (end != name + 1 && *end == '\0' && n >= 0 && n <= 31)
            return (int)n;
        return -1;
    }

    /* fp is an alias for s0 (x8) */
    if (strcmp(name, "fp") == 0) return 8;

    /* ABI names */
    for (int i = 0; i < 32; i++) {
        if (strcmp(name, abi_names[i]) == 0)
            return i;
    }
    return -1;
}