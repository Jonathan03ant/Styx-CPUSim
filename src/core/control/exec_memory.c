/**
  * @file exec_memory.c
  * @brief Memory Access Instruction Handlers
  *
  * Handlers for: LOAD, STORE
  */

#include "control_internal.h"
#include "../memory/memory.h"
#include "../decoder/decoder.h"
#include "../../common/isa_defs.h"
#include <stddef.h>

// ======= LOAD ======= //
//*Load word from memory (Rd = Memory[Rs + offset])
error_t exec_load(CPU_t* cpu, DecodedInstruction_t* insn){
    /*
        * [LOAD:2][Rd:4][Rs:4][offset:4]
        * Load word from memory at address (Rs + offset)
        * Extract: Rd (destination), Rs (base), offset
        * Calculate address: addr = Rs_value + offset
        * Read from memory, write to Rd
        * Set MAR/MDR for debugging
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register numbers and offset (I-Type B: opcode, Rd, Rs, offset)
    reg_t rd = decoded_get_rd(insn);
    reg_t rs = decoded_get_rs1(insn);
    offset4_t offset = decoded_get_immediate(insn);

    if (rd == INVALID_REG || rs == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read base address from rs
    regval_t base_addr;
    error_t err = reg_read(cpu->registers, rs, &base_addr);
    if (err != ERR_OK) {
        return err;
    }

    // Calculate effective address
    addr_t addr = base_addr + offset;

    cpu->MAR = addr;

    // Read word from memory
    word_t data;
    err = mem_read_word(cpu->memory, addr, &data);

    if (err != ERR_OK) {
        return err;
    }

    cpu->MDR = data;

    // Write data to destination register
    err = reg_write(cpu->registers, rd, data);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

// ======= STORE ======= //
//* Store word to memory (Memory[Rs + offset] = Rd)
error_t exec_store(CPU_t* cpu, DecodedInstruction_t* insn)
{
    /*
        * [STORE:3][Rd:4][Rs:4][offset:4]
        * Store word to memory at address (Rs + offset)
        * Extract: Rd (source data), Rs (base), offset
        * Calculate address: addr = Rs_value + offset
        * Read data from Rd, write to memory
        * Set MAR/MDR for debugging
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    // Extract register numbers and offset (I-Type B: opcode, Rd, Rs, offset)
    reg_t rd = decoded_get_rd(insn);
    reg_t rs = decoded_get_rs1(insn);
    offset4_t offset = decoded_get_immediate(insn);

    if (rd == INVALID_REG || rs == INVALID_REG) {
        return ERR_INVALID_REGISTER;
    }

    // Read base address from rs
    regval_t base_addr;
    error_t err = reg_read(cpu->registers, rs, &base_addr);
    if (err != ERR_OK) {
        return err;
    }

    // Read data from rd (the value we want to store)
    word_t data;
    err = reg_read(cpu->registers, rd, &data);
    if (err != ERR_OK) {
        return err;
    }

    // Calculate effective address
    addr_t addr = base_addr + offset;

    cpu->MAR = addr;
    cpu->MDR = data;

    // Write to memory
    err = mem_write_word(cpu->memory, addr, data);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}
