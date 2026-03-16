#include "../src/encode.h"
#include <stdio.h>

static int pass = 0, fail = 0;
#define CHECK(got, expected, msg) \
    do { uint32_t g=(got),e=(expected); \
         if(g==e){pass++;} \
         else{fprintf(stderr,"FAIL %s: got 0x%08X expected 0x%08X\n",msg,g,e);fail++;} \
    } while(0)

int main(void) {
    /* add x1, x2, x3  →  0x003100B3 */
    CHECK(encode_r(0x33, 0x0, 0x00, 1, 2, 3), 0x003100B3u, "add x1,x2,x3");

    /* sub x1, x2, x3  →  0x403100B3 */
    CHECK(encode_r(0x33, 0x0, 0x20, 1, 2, 3), 0x403100B3u, "sub x1,x2,x3");

    /* addi x1, x0, 5  →  0x00500093 */
    CHECK(encode_i(0x13, 0x0, 1, 0, 5), 0x00500093u, "addi x1,x0,5");

    /* addi x1, x0, -1 → 0xFFF00093 */
    CHECK(encode_i(0x13, 0x0, 1, 0, -1), 0xFFF00093u, "addi x1,x0,-1");

    /* lw x1, 0(x2)  →  0x00012083 */
    CHECK(encode_i(0x03, 0x2, 1, 2, 0), 0x00012083u, "lw x1,0(x2)");

    /* sw x3, 8(x2)  →  0x00312423 */
    CHECK(encode_s(0x23, 0x2, 2, 3, 8), 0x00312423u, "sw x3,8(x2)");

    /* beq x1, x2, +8  →  0x00208463 */
    CHECK(encode_b(0x63, 0x0, 1, 2, 8), 0x00208463u, "beq x1,x2,+8");

    /* jal x1, 0  →  0x000000EF */
    CHECK(encode_j(0x6F, 1, 0), 0x000000EFu, "jal x1,0");

    printf("encode tests: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}