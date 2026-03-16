#include "../src/symbols.h"
#include <stdio.h>

static int pass = 0, fail = 0;
#define CHECK(cond, msg) \
    do { if(cond){pass++;}else{fprintf(stderr,"FAIL: %s\n",msg);fail++;} } while(0)

int main(void) {
    SymbolTable tbl;
    symbols_init(&tbl);

    CHECK(symbols_add(&tbl, "start", 0)  == 0, "add start");
    CHECK(symbols_add(&tbl, "loop",  12) == 0, "add loop");
    CHECK(symbols_add(&tbl, "done",  24) == 0, "add done");

    CHECK(symbols_find(&tbl, "start") == 0,  "find start");
    CHECK(symbols_find(&tbl, "loop")  == 12, "find loop");
    CHECK(symbols_find(&tbl, "done")  == 24, "find done");
    CHECK(symbols_find(&tbl, "missing") == -1, "find missing");

    /* Duplicate detection */
    CHECK(symbols_add(&tbl, "loop", 99) == -1, "duplicate label");
    /* After failed duplicate, original value unchanged */
    CHECK(symbols_find(&tbl, "loop") == 12, "original preserved");

    printf("symbols tests: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}