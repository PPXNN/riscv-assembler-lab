#include "../src/registers.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    int pass = 0, fail = 0;

#define CHECK(expr, expected) \
    do { int got = (expr); \
         if (got == (expected)) { pass++; } \
         else { fprintf(stderr, "FAIL: %s → %d (expected %d)\n", #expr, got, expected); fail++; } \
    } while(0)

    /* xN names */
    CHECK(parse_register("x0"),  0);
    CHECK(parse_register("x1"),  1);
    CHECK(parse_register("x15"), 15);
    CHECK(parse_register("x31"), 31);

    /* ABI names */
    CHECK(parse_register("zero"), 0);
    CHECK(parse_register("ra"),   1);
    CHECK(parse_register("sp"),   2);
    CHECK(parse_register("gp"),   3);
    CHECK(parse_register("tp"),   4);
    CHECK(parse_register("t0"),   5);
    CHECK(parse_register("t1"),   6);
    CHECK(parse_register("t2"),   7);
    CHECK(parse_register("s0"),   8);
    CHECK(parse_register("fp"),   8);
    CHECK(parse_register("s1"),   9);
    CHECK(parse_register("a0"),  10);
    CHECK(parse_register("a7"),  17);
    CHECK(parse_register("t6"),  31);

    /* Invalid */
    CHECK(parse_register("x32"), -1);
    CHECK(parse_register("x-1"), -1);
    CHECK(parse_register("foo"), -1);
    CHECK(parse_register(""),    -1);
    CHECK(parse_register(NULL),  -1);

    printf("register tests: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}