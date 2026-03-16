#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/* Assemble the file at 'input_path'.
   Writes hex machine code to 'output_path' (one 32-bit word per line).
   Returns 0 on success, non-zero on error. */
int assemble_file(const char *input_path, const char *output_path);

#endif /* ASSEMBLER_H */