#include "parser.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/* Trim leading/trailing whitespace in-place and return pointer */
static char *trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end-1))) end--;
    *end = '\0';
    return s;
}

int parse_line(const char *line, ParsedLine *out) {
    memset(out, 0, sizeof(*out));

    /* Work on a mutable copy */
    char buf[256];
    strncpy(buf, line, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';

    /* Strip inline comment (# ...) */
    char *comment = strchr(buf, '#');
    if (comment) *comment = '\0';

    char *p = trim(buf);
    if (*p == '\0') return 0;   /* blank / comment-only line */

    /* Check for a label (token ending with ':') */
    char *colon = strchr(p, ':');
    if (colon) {
        /* label is everything before the colon */
        *colon = '\0';
        char *lbl = trim(p);
        strncpy(out->label, lbl, MAX_TOKEN-1);
        p = trim(colon + 1);
        if (*p == '\0') return 0; /* label-only line – caller records it */
    }

    /* First token = mnemonic */
    char *tok = strtok(p, " \t,()");
    if (!tok) return 0;
    strncpy(out->mnemonic, tok, MAX_TOKEN-1);

    /* Remaining tokens = operands.
       Memory instructions like lw x1, 8(x2) produce tokens: x1  8  x2  – that's fine. */
    while ((tok = strtok(NULL, " \t,()")) != NULL && out->num_operands < MAX_OPERANDS) {
        char *trimmed = trim(tok);
        if (*trimmed == '\0') continue;
        strncpy(out->operands[out->num_operands++], trimmed, MAX_TOKEN-1);
    }
    return 1;
}