#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

uint32_t encode_r(unsigned opcode, unsigned funct3, unsigned funct7,
                  unsigned rd, unsigned rs1, unsigned rs2);

uint32_t encode_i(unsigned opcode, unsigned funct3,
                  unsigned rd, unsigned rs1, int imm);

uint32_t encode_s(unsigned opcode, unsigned funct3,
                  unsigned rs1, unsigned rs2, int imm);

uint32_t encode_b(unsigned opcode, unsigned funct3,
                  unsigned rs1, unsigned rs2, int imm);

uint32_t encode_u(unsigned opcode, unsigned rd, int imm);

uint32_t encode_j(unsigned opcode, unsigned rd, int imm);

#endif /* ENCODE_H */