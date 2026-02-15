#ifndef STYX_ISA_DEFS_H
#define STYX_ISA_DEFS_H

#include "types.h"

/*============================================================================
  * STYX ISA DEFINITIONS
  *
  * Pure ISA specification - opcodes, formats, field extraction
  *============================================================================*/

/*----------------------------------------------------------------------------
  * OPCODE DEFINITIONS
  *----------------------------------------------------------------------------*/
typedef enum {
    OP_NOP      = 0x0,      // I-Type A - No operation (ADDI S0, 0)

    // Data Mgmt
    OP_MOV      = 0x1,      // R-Type - Move register to register
    OP_LOAD     = 0x2,      // I-Type B - Load from memory
    OP_STORE    = 0x3,      // I-Type B - Store from memory

    // Arithmetic
    OP_ADD      = 0x4,      // R-Type - Addition
    OP_SUB      = 0x5,      // R-Type - Subtraction
    OP_MUL      = 0x6,      // R-Type - Multiplication
    OP_DIV      = 0x7,      // R-Type - Division
    OP_ADDI     = 0x8,      // I-Type A - Add immediate

    // Comparision
    OP_CMP      = 0x9,      // R-Type - Compare (set FLAGS)

    // Control Flow
    OP_JMP      = 0xA,      // J-Type - Unconditional jump
    OP_JZ       = 0xB,      // J-Type - Jump if zero
    OP_JNZ      = 0xC,      // J-Type - Jump if not zero
    OP_CALL     = 0xD,      // J-Type - Call subroutine
    OP_RET      = 0xE,      // J-Type - Return from subroutine

    // Immediate
    OP_LUI      = 0xF,      // I-Type A - Load upper immediate

    // Count for bounds checking
    OP_COUNT = 16
} Opcode_e;


/*----------------------------------------------------------------------------
  * INSTRUCTION FORMATS
  *----------------------------------------------------------------------------*/
typedef enum {
    INSN_FORMAT_R,          // R-type   [op:4][rd:4][rs1:4][rs2:4]
    INSN_FORMAT_I_A,        // I-Type A [op:4][rd:4][imm:8] (ADDI, LUI)
    INST_FORMAT_I_B,        // I-Type B [op:4][rd:4][rs:4][offset:4] (LOAD, STORE)
    INSN_FORMAT_J,          // J-Type   [op:4][addr:12]
    INSN_FORMAT_INVALID
} InsnFormat_e;


/*----------------------------------------------------------------------------
  * FIELD EXTRACTION FUNCTIONS
  *
  * Styx instruction layout: [opcode:4][operands:12]
  * Bits numbered 15 (MSB) down to 0 (LSB)
  * Bit:  15 14 13 12 | 11 10 9 8 | 7 6 5 4 | 3 2 1 0
            [ opcode ]  [  rd  ]     [ rs1 ]   [ rs2 ]
  *----------------------------------------------------------------------------*/

// Extract opcode (bits 15-12)
static inline opcode_t insn_get_opcode(insn_t insn){
    return (insn >> 12) & 0xF;
}

// R-type field extraction
// [op:4][rd:4][rs1:4][rs2:4]
// extract rd
static inline reg_t insn_get_r_rd(insn_t insn){
    return (insn >> 8) & 0xF;
}

// extract rs1
static inline reg_t insn_get_r_rs1(insn_t insn){
    return (insn >> 4) & 0xF;
}

// extract rs2
static inline reg_t insn_get_r_rs2(insn_t insn){
    return insn & 0xF;
}

// I-type A field extraction
// [op:4][rd:4][imm:8]
// extract i_a_rd
static inline reg_t insn_get_i_a_rd(insn_t insn){
    return (insn >> 8) & 0xF;
}

// extract imm8
static inline imm8_t insn_get_i_a_imm(insn_t insn){
    return insn & 0xFF;
}






#endif /* STYX_ISA_DEFS_H */