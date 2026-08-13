/**
  * @file exec_arith.c
  * @brief Arithmetic Instruction Handlers
  *
  * Handlers for: ADD, SUB, MUL, DIV, ADDI
  */

#include "isa_handlers.h"
#include "alu.h"
#include "decoder.h"
#include "common/isa_defs.h"
#include <stddef.h>

// ======= ADD ======= //
//* ADD two rs registers, write to rd
error_t exec_add(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [ADD:4][rd:4][rs1:4][rs2:4]
        * Add rs1, rs2, write to rd
        * Extract all registers
        * Read the values on those registers
        * CALL ALU and write to rd
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register numbers (R-Type: opcode, Rd, Rs1, Rs2)
    reg_t rd = decoded_get_rd(insn);
    reg_t rs1 = decoded_get_rs1(insn);
    reg_t rs2 = decoded_get_rs2(insn);

    if (rd == INVALID_REG || rs1 == INVALID_REG || rs2 == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read values from register file
    regval_t val1, val2;
    error_t err = reg_read(cpu->registers, rs1, &val1);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_read(cpu->registers, rs2, &val2);
    if (err != ERR_OK) {
        return err;
    }

    // Call ALU to perfrom addition
    regval_t result;
    err = alu_add(val1, val2, &result);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_write(cpu->registers, rd, result);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

// ======= SUB ======= //
//* Subtract two rs registers, write to rd
error_t exec_sub(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [SUB:5][rd:4][rs1:4][rs2:4]
        * Add rs1, rs2, write to rd
        * Extract all registers
        * Read the values on those registers
        * CALL ALU and write to rd
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register numbers (R-Type: opcode, Rd, Rs1, Rs2)
    reg_t rd = decoded_get_rd(insn);
    reg_t rs1 = decoded_get_rs1(insn);
    reg_t rs2 = decoded_get_rs2(insn);

    if (rd == INVALID_REG || rs1 == INVALID_REG || rs2 == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read values from register file
    regval_t val1, val2;
    error_t err = reg_read(cpu->registers, rs1, &val1);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_read(cpu->registers, rs2, &val2);
    if (err != ERR_OK) {
        return err;
    }

    // Call ALU to perfrom addition
    regval_t result;
    err = alu_sub(val1, val2, &result);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_write(cpu->registers, rd, result);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

// ======= MUL ======= //
//* Multiply two rs registers, write to rd
error_t exec_mul(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [MUL:6][rd:4][rs1:4][rs2:4]
        * Add rs1, rs2, write to rd
        * Extract all registers
        * Read the values on those registers
        * CALL ALU and write to rd
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register numbers (R-Type: opcode, Rd, Rs1, Rs2)
    reg_t rd = decoded_get_rd(insn);
    reg_t rs1 = decoded_get_rs1(insn);
    reg_t rs2 = decoded_get_rs2(insn);

    if (rd == INVALID_REG || rs1 == INVALID_REG || rs2 == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read values from register file
    regval_t val1, val2;
    error_t err = reg_read(cpu->registers, rs1, &val1);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_read(cpu->registers, rs2, &val2);
    if (err != ERR_OK) {
        return err;
    }

    // Call ALU to perfrom addition
    regval_t result;
    err = alu_mul(val1, val2, &result);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_write(cpu->registers, rd, result);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

// ======= DIV ======= //
//* Devide two rs registers, write to rd
error_t exec_div(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [DIV:7][rd:4][rs1:4][rs2:4]
        * Add rs1, rs2, write to rd
        * Extract all registers
        * Read the values on those registers
        * CALL ALU and write to rd
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register numbers (R-Type: opcode, Rd, Rs1, Rs2)
    reg_t rd = decoded_get_rd(insn);
    reg_t rs1 = decoded_get_rs1(insn);
    reg_t rs2 = decoded_get_rs2(insn);

    if (rd == INVALID_REG || rs1 == INVALID_REG || rs2 == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read values from register file
    regval_t val1, val2;
    error_t err = reg_read(cpu->registers, rs1, &val1);
    if (err != ERR_OK) {
        return err;
    }

    err = reg_read(cpu->registers, rs2, &val2);
    if (err != ERR_OK) {
        return err;
    }

    // Call ALU to perform division
    regval_t result;
    err = alu_div(val1, val2, &result);
    // Division by zero: ALU sets result to 0, continue execution
    if (err != ERR_OK && err != ERR_DIVISION_BY_ZERO) {
        return err;
    }

    err = reg_write(cpu->registers, rd, result);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

// ======= ADDI ======= //
//* Add immediate to register (Rd = Rd + imm8)
error_t exec_addi(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [ADDI:8][rd:4][imm:8]
        * Adds lower part of register value
        * Used to build 16 bit address/values
        * Extract rd and imm value, add imm to rd (0-7)
        * write back rd
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register number and immediate value (I-Type A: opcode, Rd, imm8)
    reg_t rd = decoded_get_rd(insn);
    imm8_t imm = decoded_get_immediate(insn);

    if (rd == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read current value of rd
    regval_t rd_value;
    error_t err = reg_read(cpu->registers, rd, &rd_value);
    if (err != ERR_OK) {
        return err;
    }

    // Add Imm to currentvalue
    regval_t result;
    err = alu_add(rd_value, (regval_t)imm, &result);
    if (err != ERR_OK) {
        return err;
    }

    // Write result back to rd
    err = reg_write(cpu->registers, rd, result);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}