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

// Format decoder function pointer type
typedef error_t (*FormatDecoder_t)(insn_t raw, DecodedInstruction_t *decoded);

// Format decoder lookup table (indexed by instruction format)
static const FormatDecoder_t format_decoder_LUT[] = {
    [INSN_FORMAT_R] = decode_rtype,
    [INSN_FORMAT_I_A] = decode_itype_a,
    [INSN_FORMAT_I_B] = decode_itype_b,
    [INSN_FORMAT_J] = decode_jtype,
};

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

// Return opcode from raw_insn, calls isa wrapper
opcode_t get_opcode(insn_t instruction) {
    return insn_get_opcode(instruction);
}

reg_t get_rs(insn_t instruction, InsnFormat_e format) {
    switch (format) {
        case INSN_FORMAT_R:
            return insn_get_r_rs1(instruction);
        case INSN_FORMAT_I_B:
            return insn_get_i_b_rs(instruction);
        default:
            return INVALID_REG;
    }
}

reg_t get_rs1(insn_t instruction) {
    return insn_get_r_rs1(instruction);
}

reg_t get_rs2(insn_t instruction) {
    return insn_get_r_rs2(instruction);
}

imm8_t get_imm8(insn_t instruction) {
    return insn_get_i_a_imm(instruction);
}

offset4_t get_offset(insn_t instruction) {
    return insn_get_i_b_offset(instruction);
}

addr12_t get_addr12(insn_t instruction) {
    return insn_get_j_addr(instruction);
}

/*
  Extract destination register from decoded instruction.
*/
reg_t decoded_get_rd(const DecodedInstruction_t *decoded)
{
    switch (decoded->format) {
        case INSN_FORMAT_R:
            return decoded->fields.r_type.rd;
        case INSN_FORMAT_I_A:
            return decoded->fields.i_type_a.rd;
        case INSN_FORMAT_I_B:
            return decoded->fields.i_type_b.rd;
        case INSN_FORMAT_J:
            return INVALID_REG;
        default:
            return INVALID_REG;
    }
}

/*
  Extract src1 register from decoded instruction.
*/
reg_t decoded_get_rs1(const DecodedInstruction_t *decoded)
{
    switch (decoded->format) {
        case INSN_FORMAT_R:
            return decoded->fields.r_type.rs1;
        case INSN_FORMAT_I_A:
            return INVALID_REG;
        case INSN_FORMAT_I_B:
            return decoded->fields.i_type_b.rs;
        case INSN_FORMAT_J:
            return INVALID_REG;
        default:
            return INVALID_REG;
    }
}

/*
  Extract src2 register from decoded instruction.
*/
reg_t decoded_get_rs2(const DecodedInstruction_t *decoded)
{
    switch (decoded->format) {
        case INSN_FORMAT_R:
            return decoded->fields.r_type.rs2;

        case INSN_FORMAT_I_A:
        case INSN_FORMAT_I_B:
        case INSN_FORMAT_J:
            return INVALID_REG;
        default:
            return INVALID_REG;
        }
}

/*
  Extract omm8 (or offset) register from decoded instruction.
*/
reg_t decoded_get_immediate(const DecodedInstruction_t *decoded)
{
    switch (decoded->format) {
        case INSN_FORMAT_I_A:
            return decoded->fields.i_type_a.imm;
        case INSN_FORMAT_I_B:
            return decoded->fields.i_type_b.offset;

        case INSN_FORMAT_R:
        case INSN_FORMAT_J:
            return INVALID_REG;
        default:
        return INVALID_REG;
    }
}

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
  Validate given opcode is valid
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

/*
    * Extract all rtype registers
    * Populate decoder, set the rtype field
*/
static error_t decode_rtype(insn_t raw, DecodedInstruction_t *decoded)
{
    decoded->fields.r_type.rd = insn_get_i_a_rd(raw);
    decoded->fields.r_type.rs1 = insn_get_r_rs1(raw);
    decoded->fields.r_type.rs2 = insn_get_r_rs2(raw);
    return ERR_OK;
}

/*
    * Extract itype_a; rd and imm
    * Populate decoder
*/
static error_t decode_itype_a(insn_t raw, DecodedInstruction_t *decoded)
{
    decoded->fields.i_type_a.rd = insn_get_i_a_rd(raw);
    decoded->fields.i_type_a.imm = insn_get_i_a_imm(raw);
    return ERR_OK;
}

/*
    * Extract itype_b; rd, rs, and offset
    * Populate decoder
*/
static error_t decode_itype_b(insn_t raw, DecodedInstruction_t *decoded) {
    decoded->fields.i_type_b.rd = insn_get_r_rd(raw);
    decoded->fields.i_type_b.rs = insn_get_i_b_rs(raw);
    decoded->fields.i_type_b.offset = insn_get_i_b_offset(raw);
    return ERR_OK;
}

/*
    * Extract jtype; jump to address (addr)
    * Populate decoder
*/
static error_t decode_jtype(insn_t raw, DecodedInstruction_t *decoded) {
    decoded->fields.j_type.addr = insn_get_j_addr(raw);
    return ERR_OK;
}

/*
    * Main orchestreture AT RUNTIME
    * gets raw instruction from memory, parses and validate the opcode
    * determine the opcode format form the LUT, store opcode and format
    * despatch the format decoder, which sets the format fields
*/
error_t decode_instruction(insn_t raw_instruction, DecodedInstruction_t *decoded)
{
    if (decoded == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract opcode from raw instruction
    opcode_t opcode = insn_get_opcode(raw_instruction);
    if (!is_valid_opcode(opcode)){
        return ERR_INVALID_OPCODE;
    }

    InsnFormat_e format = opcode_LUT[opcode];
    decoded->opcode = opcode;
    decoded->format = format;

    // Dispatch to appropriate format deocder
    error_t err = format_decoder_LUT[format](raw_instruction, decoded);
    if (err != ERR_OK) {
        return err;
    }

    if (!is_valid_instruction(decoded)){
        return ERR_INVALID_INSTRUCTION;
    }
}