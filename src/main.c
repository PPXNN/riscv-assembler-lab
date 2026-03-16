#include "assembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <input.s> [output.hex]\n", argv[0]);
        return 1;
    }

    /* Build default output filename: replace .s with .hex */
    char outbuf[256];
    if (argc == 3) {
        strncpy(outbuf, argv[2], sizeof(outbuf)-1);
    } else {
        strncpy(outbuf, argv[1], sizeof(outbuf)-5);
        char *dot = strrchr(outbuf, '.');
        if (dot) *dot = '\0';
        strncat(outbuf, ".hex", 5);
    }

    if (assemble_file(argv[1], outbuf) != 0) {
        fprintf(stderr, "Assembly failed.\n");
        return 1;
    }

    printf("Assembled '%s' → '%s'\n", argv[1], outbuf);
    return 0;
}