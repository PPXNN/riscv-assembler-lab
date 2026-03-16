#ifndef PARSER_H
#define PARSER_H

#define MAX_OPERANDS 4
#define MAX_TOKEN    64

typedef struct {
    char label   [MAX_TOKEN];   /* may be empty string */
    char mnemonic[MAX_TOKEN];   /* e.g. "add" */
    char operands[MAX_OPERANDS][MAX_TOKEN];
    int  num_operands;
} ParsedLine;

/* Parse one line of assembly text.
   Returns 1 on success (at least a mnemonic found), 0 if line is blank/comment. */
int parse_line(const char *line, ParsedLine *out);

#endif /* PARSER_H */