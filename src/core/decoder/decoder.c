/**
  * @file decode.c
  * @brief Main instruction decoder implementation
  *
  * Dispatches to format-specific decoders and provides
  * high-level decoder API implementation.
  */

#include "decoder.h"
#include "../../common/isa_defs.h"
#include "../../core/registers/registers.h"
#include "../../utils/errors.h"
#include <stddef.h>

// Forward declarations for internal format decoders
static error_t decode_rtype(insn_t raw, DecodedInstruction_t *decoded);
static error_t decode_itype_a(insn_t raw, DecodedInstruction_t *decoded);
static error_t decode_itype_b(insn_t raw, DecodedInstruction_t *decoded);
static error_t decode_jtype(insn_t raw, DecodedInstruction_t *decoded);

// Opcode to format lookup table
static const InsnFormat_e opcode_LUT[16] = {
    [OP_NOP]   = INSN_FORMAT_I_A,           // Opcode 0 (NOP) is I-Type A
    [OP_MOV]   = INSN_FORMAT_R,             // Opcode 1 (MOV) is R-Type
    [OP_LOAD]  = INSN_FORMAT_I_B,           // Opcode 2 (LOAD) is I-Type B
    [OP_STORE] = INSN_FORMAT_I_B,           // Opcode 3 (STORE) is I-Type B
    [OP_ADD]   = INSN_FORMAT_R,             // Opcode 4 (ADD) is R-Type
    [OP_SUB]   = INSN_FORMAT_R,             // Opcode 5 (SUB) is R-Type
    [OP_MUL]   = INSN_FORMAT_R,             // Opcode 6 (MUL) is R-Type
    [OP_DIV]   = INSN_FORMAT_R,             // Opcode 7 (DIV) is R-Type
    [OP_CMP]   = INSN_FORMAT_R,             // Opcode 8 (CMP) is R-Type
    [OP_JMP]   = INSN_FORMAT_J,             // Opcode 9 (JMP) is J-Type
    [OP_JZ]    = INSN_FORMAT_J,             // Opcode 10 (JZ) is J-Type
    [OP_JNZ]   = INSN_FORMAT_J,             // Opcode 11 (JNZ) is J-Type
    [OP_CALL]  = INSN_FORMAT_J,             // Opcode 12 (CALL) is J-Type
    [OP_RET]   = INSN_FORMAT_J,             // Opcode 13 (RET) is J-Type
    [OP_ADDI]  = INSN_FORMAT_I_A,           // Opcode 14 (ADDI) is I-Type A
    [OP_LUI]   = INSN_FORMAT_I_A,           // Opcode 15 (LUI) is I-Type A
};

/*
  Validate a decoded instruction opcode and format
  * TO be adjusted later as needed
*/
bool is_valid_instruction(const DecodedInstruction_t *decoded)
{
    if (!is_valid_opcode(decoded->opcode)){
        return false;
    }

    InsnFormat_e expected_format = get_inst_format(decoded->opcode);
    if (decoded->format != expected_format) {
        return false;
    }

    return true;
}

/*
  Validate giben opcode is valide
  Check with OP_COUNT
*/
bool is_valid_opcode(opcode_t opcode)
{
    return opcode < OP_COUNT;
}

/*
  Validate given register is valid
  Calls register validation function
*/
bool is_valid_register(reg_t reg_num)
{
    return reg_is_valid_num(reg_num);
}

/*
  InsnFormat_e LUT
*/
InsnFormat_e get_inst_format(opcode_t opcode)
{
    if (is_valid_opcode(opcode)){
        return opcode_LUT[opcode];
    } else {
        return INSN_FORMAT_INVALID;
    }
}

/*
  Check if instruction has immediate field (I-Types only)
*/
bool instruction_has_imm(opcode_t opcode)
{
    InsnFormat_e format = get_inst_format(opcode);
    return (format == INSN_FORMAT_I_A || format == INSN_FORMAT_I_B);
}

/*
  Check if instruction is LOAD/STORE
*/
bool instruction_is_mem_access(opcode_t opcode)
{
    InsnFormat_e format = get_inst_format(opcode);
    return (format == INSN_FORMAT_I_B);
}

/*
  Check if instruction is control flow
*/
bool instruction_is_ctl_flow(opcode_t opcode)
{
    InsnFormat_e format = get_inst_format(opcode);
    return (format == INSN_FORMAT_J);
}