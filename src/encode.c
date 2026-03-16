#include "encode.h"

/* Helpers – extract a bit range [hi:lo] from value */
#define BITS(v, hi, lo)  (((unsigned)(v) >> (lo)) & ((1u << ((hi)-(lo)+1)) - 1))

/* R-type: [31:25]=funct7 [24:20]=rs2 [19:15]=rs1 [14:12]=funct3 [11:7]=rd [6:0]=opcode */
uint32_t encode_r(unsigned opcode, unsigned funct3, unsigned funct7,
                  unsigned rd, unsigned rs1, unsigned rs2) {
    return ((funct7 & 0x7F) << 25)
         | ((rs2    & 0x1F) << 20)
         | ((rs1    & 0x1F) << 15)
         | ((funct3 & 0x07) << 12)
         | ((rd     & 0x1F) <<  7)
         | ( opcode & 0x7F);
}

/* I-type: [31:20]=imm[11:0] [19:15]=rs1 [14:12]=funct3 [11:7]=rd [6:0]=opcode */
uint32_t encode_i(unsigned opcode, unsigned funct3,
                  unsigned rd, unsigned rs1, int imm) {
    /* For shift immediates (slli/srli/srai), caller puts funct7 into upper imm bits */
    uint32_t imm12 = (uint32_t)(imm & 0xFFF);
    return (imm12 << 20)
         | ((rs1    & 0x1F) << 15)
         | ((funct3 & 0x07) << 12)
         | ((rd     & 0x1F) <<  7)
         | ( opcode & 0x7F);
}

/* S-type: [31:25]=imm[11:5] [24:20]=rs2 [19:15]=rs1 [14:12]=funct3 [11:7]=imm[4:0] [6:0]=opcode */
uint32_t encode_s(unsigned opcode, unsigned funct3,
                  unsigned rs1, unsigned rs2, int imm) {
    uint32_t u = (uint32_t)(imm & 0xFFF);
    return (BITS(u,11,5) << 25)
         | ((rs2    & 0x1F) << 20)
         | ((rs1    & 0x1F) << 15)
         | ((funct3 & 0x07) << 12)
         | (BITS(u, 4,0) <<  7)
         | ( opcode & 0x7F);
}

/* B-type immediate is in multiples of 2 bytes.
   Encoding: [31]=imm[12] [30:25]=imm[10:5] [11:8]=imm[4:1] [7]=imm[11] */
uint32_t encode_b(unsigned opcode, unsigned funct3,
                  unsigned rs1, unsigned rs2, int imm) {
    uint32_t u = (uint32_t)(imm & 0x1FFE); /* keep bits [12:1] */
    return (BITS(imm,12,12) << 31)
         | (BITS(u, 10, 5) << 25)
         | ((rs2    & 0x1F) << 20)
         | ((rs1    & 0x1F) << 15)
         | ((funct3 & 0x07) << 12)
         | (BITS(u,  4, 1) <<  8)
         | (BITS(imm,11,11) <<  7)
         | ( opcode & 0x7F);
}

/* U-type: [31:12]=imm[31:12] [11:7]=rd [6:0]=opcode */
uint32_t encode_u(unsigned opcode, unsigned rd, int imm) {
    return ((uint32_t)(imm & 0xFFFFF000))
         | ((rd & 0x1F) << 7)
         | ( opcode & 0x7F);
}

/* J-type: imm[20|10:1|11|19:12] packed into [31:12] */
uint32_t encode_j(unsigned opcode, unsigned rd, int imm) {
    return (BITS(imm,20,20) << 31)
         | (BITS(imm,10, 1) << 21)
         | (BITS(imm,11,11) << 20)
         | (BITS(imm,19,12) << 12)
         | ((rd & 0x1F)     <<  7)
         | ( opcode & 0x7F);
}