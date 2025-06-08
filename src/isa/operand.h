#ifndef OPERAND_H
#define OPERAND_H

#include <stdint.h>
#include "isa_if.h"  

// Register-type operand (R-type): 3 registers (4 bits each)
typedef struct {
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
} OperandR;

// Immediate-type operand (I-type): 2 registers + 4-bit immediate
typedef struct {
    uint8_t rd;
    uint8_t rs1;
    uint8_t imm;
} OperandI;

// Jump-type operand (J-type): full 12-bit immediate
typedef struct {
    uint16_t imm;
} OperandJ;

// Function pointer type for operand decoders
typedef void (*OperandDecoder)(uint16_t raw_operand, void *OperandStruct);

// Concrete decoder functions for each type
void decode_operand_r(uint16_t raw, void *Out);
void decode_operand_i(uint16_t raw, void *Out);
void decode_operand_j(uint16_t raw, void *Out);

// Dispatch table: indexed by operand type (R/I/J)
extern OperandDecoder operand_decoder_table[OPERAND_T_COUNT];

#endif // OPERAND_H
