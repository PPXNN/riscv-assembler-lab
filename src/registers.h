#ifndef REGISTERS_H
#define REGISTERS_H

/* Parse a register name like "x0".."x31" or ABI names (zero, ra, sp, ...).
   Returns the register number (0-31), or -1 on error. */
int parse_register(const char *name);

#endif /* REGISTERS_H */