#ifndef OPERAND_H
#define OPERAND_H

#include <stdint.h>

// Operand structures
typedef struct {
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
} OperandR;

typedef struct {
    uint8_t rd;
    uint8_t rs1;
    uint8_t imm;
} OperandI;

typedef struct {
    uint16_t imm;
} OperandJ;

// Function pointer type for operand decoding
typedef void (*OperandDecoder)(uint16_t raw, void *out_operand);

// Array of decoder function pointers indexed by ISAType
extern OperandDecoder operand_decoder_table[3];

// Function declarations (optional direct use)
void decode_r_operand(uint16_t raw, void *out);
void decode_i_operand(uint16_t raw, void *out);
void decode_j_operand(uint16_t raw, void *out);

#endif // OPERAND_H
