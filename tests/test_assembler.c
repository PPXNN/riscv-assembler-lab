#include "../src/assembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int pass = 0, fail = 0;

#define CHECK(cond, msg) \
    do { if(cond){pass++;}else{fprintf(stderr,"FAIL: %s\n",msg);fail++;} } while(0)

static int run_test(const char *asm_text, const char **expected_hex, int num_expected) {
    FILE *f = fopen("_test_input.s", "w");
    if (!f) return -1;
    fputs(asm_text, f);
    fclose(f);

    if (assemble_file("_test_input.s", "_test_output.hex") != 0)
        return -1;

    f = fopen("_test_output.hex", "r");
    if (!f) return -1;

    char line[32];
    int i = 0;
    while (fgets(line, sizeof(line), f) && i < num_expected) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strcasecmp(line, expected_hex[i]) != 0) {
            fprintf(stderr, "  line %d: got '%s'  expected '%s'\n", i, line, expected_hex[i]);
            fclose(f);
            return -1;
        }
        i++;
    }
    fclose(f);
    return (i == num_expected) ? 0 : -1;
}

int main(void) {
    int rc;

    { const char *hex[] = { "0x00500093" };
      rc = run_test("addi x1, x0, 5\n", hex, 1);
      CHECK(rc == 0, "addi x1,x0,5"); }

    { const char *hex[] = { "0x002081B3" };
      rc = run_test("add x3, x1, x2\n", hex, 1);
      CHECK(rc == 0, "add x3,x1,x2"); }

    { const char *hex[] = { "0x00100093","0x00200113","0x00208463","0x00300193" };
      const char *src = "addi x1, x0, 1\naddi x2, x0, 2\nbeq  x1, x2, done\naddi x3, x0, 3\ndone:\n";
      rc = run_test(src, hex, 4);
      CHECK(rc == 0, "branch label resolution"); }

    { const char *hex[] = { "0x00112423","0x00812083" };
      rc = run_test("sw x1, 8(x2)\nlw x1, 8(x2)\n", hex, 2);
      CHECK(rc == 0, "sw/lw encoding"); }

    { const char *hex[] = { "0x000000EF" };
      rc = run_test("jal x1, 0\n", hex, 1);
      CHECK(rc == 0, "jal x1, 0"); }

    remove("_test_input.s");
    remove("_test_output.hex");

    printf("assembler integration tests: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}