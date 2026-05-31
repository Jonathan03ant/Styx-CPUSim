/**
  * @file decoder.h
  * @brief decoder subsystem public API
  *
  * Instruction decoding logic, main struct Decoded_instruction_t
  */

#ifndef DECODER_H
#define DECODER_H

#include "../../common/isa_defs.h"
#include "../../common/types.h"
#include "../../utils/errors.h"

/**
  * STYX INSTRUCTION DECODER
  * High-level decoder API - abstracts ISA field extraction
  * External modules (Control Unit, Disassembler) should use THIS header,
**/
error_t decode_instruction(insn_t raw_instruction, DecodedInstruction_t *decoded);
opcode_t get_opcode(insn_t instruction);

reg_t get_rs(insn_t instruction, InsnFormat_e format);
reg_t get_rs1(insn_t instruction);
reg_t get_rs2(insn_t instruction);
imm8_t get_imm8(insn_t instruction);
offset4_t get_offset(insn_t instruction);                   // offset (I-Type B: LOAD, STORE, bits 3-0)
addr12_t get_addr12(insn_t instruction);                    // 12-bit address (J-Type: JMP, JZ, JNZ, CALL, RET, bits 11-0)

/**
  * SMART FIELD EXTRACTION (from DecodedInstruction_t struct)
  * These navigate the union correctly based on instruction format.
  * Used after decode_instruction() to safely access fields.
  **/
reg_t decoded_get_rd(const DecodedInstruction_t *decoded);
reg_t decoded_get_rs1(const DecodedInstruction_t *decoded);
reg_t decoded_get_rs2(const DecodedInstruction_t *decoded);
reg_t decoded_get_immediate(const DecodedInstruction_t *decoded);

/**
  * Validation functions
 */
bool is_valid_instruction(const DecodedInstruction_t *decoded);
bool is_valid_opcode(opcode_t opcode);
bool is_valid_register(reg_t reg_num);

/**
  * Instruction access API's
 */
InsnFormat_e get_inst_format(opcode_t opcode);
bool instruction_has_imm(opcode_t opcode);
bool instruction_is_mem_access(opcode_t opcode);
bool instruction_is_ctl_flow(opcode_t opcode);

#endif // DECODER_H