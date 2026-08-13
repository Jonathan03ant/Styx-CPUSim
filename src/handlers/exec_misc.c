/**
  * @file exec_misc.c
  * @brief Miscellaneous Instruction Handlers
  *
  * Handlers for: NOP, MOV, CMP, LUI
  */

#include "isa_handlers.h"
#include "alu.h"
#include "decoder.h"
#include "common/isa_defs.h"
#include <stddef.h>

// ======= NOP ======= //
error_t exec_nop(CPU_t* cpu, DecodedInstruction_t* insn)
{
    //*Nop does nothing, just verify inputs and retrun
    //*Maybe a placeholder for other inst
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    return ERR_OK;
}

// ======= MOV ======= //
//* Register to register COPY
error_t exec_mov(CPU_t* cpu, DecodedInstruction_t* insn)
{
    /*
        * [OP_MOV:4][Rd:4][Rs1:4][Rs2:4]
        * Extract registers from insn (source and destination)
        * Read the source register from CPU register
        * write it to the destinatino register
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register numbers (address)
    reg_t rd = decoded_get_rd(insn);
    reg_t rs = decoded_get_rs1(insn);

    // Read source register
    regval_t value;
    error_t err = reg_read(cpu->registers, rs, &value);
    if (err != ERR_OK) {
        return err;
    }

    // Write to destination
    err = reg_write(cpu->registers, rd, value);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

// ======= CPM ======= //
//*Compare two registers and set flags
error_t exec_cmp(CPU_t* cpu, DecodedInstruction_t* insn){
    /*
        * [OP_CMP:4][Rd:4][Rs1:4][Rs2:4]
        * Extract registers from insn (source1 and source2)
        * Read the source register from CPU register
        * CALL ALU for comparision, and set flags
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    reg_t rs1 = decoded_get_rs1(insn);
    reg_t rs2 = decoded_get_rs2(insn);

    if (rs1 == INVALID_REG || rs2 == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read both values on rs1 and rs2
    regval_t val1, val2;
    error_t err = reg_read(cpu->registers, rs1, &val1);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_read(cpu->registers, rs2, &val2);
    if (err != ERR_OK) {
        return err;
    }

    // CALL ALU to perform comp
    flags_t result_flags;
    err = alu_compare(val1, val2, &result_flags);
    if (err != ERR_OK) {
        return err;
    }

    reg_set_flags(cpu->registers, result_flags);
    if (err != ERR_OK) {
        return err;
    }
    return ERR_OK;
}

// ======= LUI ======= //
//* Loads upper imm (8bit) to a registre value
error_t exec_lui(CPU_t* cpu, DecodedInstruction_t* insn)
{
    /*
        * [LUI:F][rd:4][imm:8]
        * Loads upper part of register value (8bit)
        * Used to build 16bit addresses/values across two instructions
        * Extract rd and imm value
        * write imm value to rd

    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract imm and rd values
    reg_t rd = decoded_get_rd(insn);
    imm8_t imm = decoded_get_immediate(insn);

    if (rd == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Loader uppper 8 bits: result imm << 8
    // lowerr 8 bits become zero (ADDI will be used)
    regval_t value = (regval_t) (imm << 8);

    error_t err = reg_write(cpu->registers, rd, value);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;

}

