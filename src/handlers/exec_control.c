/**
  * @file exec_control.c
  * @brief Control Flow Instruction Handlers
  *
  * Handlers for: JMP, JZ, JNZ, CALL, RET
  */

#include "isa_handlers.h"
#include "registers.h"
#include "decoder.h"
#include "common/isa_defs.h"
#include "common/reg_defs.h"
#include <stddef.h>

// ======= JMP ======= //
//*JMP to 12bit address (0x0000 to 0x0FFF (0 to 4095))
error_t exec_jmp(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [JMP:A][addr:12]
        * JMP to limited address, but fits code region 0x0100 to 0x1FFF
        * Extract address, then write it to the PC
    */
    addr12_t target = decoded_get_immediate(insn);
    error_t err = reg_write_pc(cpu->registers, (addr_t)target);

    return err;
}

// ======= JZ ======= //
//*Jump if zero flag is set
error_t exec_jz(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [JZ:B][addr:12]
        * JMP to address if Z flag is set
        * Extract address,
        * Check if FLAG_Z is set
        * if set: PC = addr, if not: do nothing (pc auto imcrements)
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    addr12_t target = decoded_get_immediate(insn);

    // Read FLAGS register
    flags_t flags =  reg_get_flags(cpu->registers);

    // Check ig Z flag is set
    if (flags & FLAG_Z) {
        error_t err = reg_write_pc(cpu->registers, (addr_t)target);
        if (err != ERR_OK) {
            return err;
        }
    } // Else: Z flag is not set, do nohing

    return ERR_OK;
}

// ======= JNZ ======= //
//*Jump if zero flag is NOT set
error_t exec_jnz(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [JNZ:C][addr:12]
        * JMP to address if Z flag is NOT set
        * Extract address,
        * Check if FLAG_Z is clear
        * if clear: PC = addr, if not: do nothing (pc auto imcrements)
    */
    if (cpu == NULL || insn == NULL){
        return ERR_NULL_POINTER;
    }

    addr12_t target = decoded_get_immediate(insn);

    // Read FLAGS register
    flags_t flags =  reg_get_flags(cpu->registers);

    // Check ig Z flag is set
    if (!(flags & FLAG_Z)) {
        //Z flag is clear, JUMP.
        error_t err = reg_write_pc(cpu->registers, (addr_t)target);
        if (err != ERR_OK) {
            return err;
        }
    } // Else: Z flag is not set, do nohing

    return ERR_OK;
}

// ======= CALL ======= //
/** Call subroutine - save return address and jump */
error_t exec_call(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [CALL:D][addr:12]
        * Call subroutine at addr
        * Save return address (PC + 2) to S15 (RA)
        * Jump to target address
        *
        * S15 = PC + 2 (next instruction after CALL)
        * PC = addr
    */
    if (cpu == NULL || insn == NULL) {
        return ERR_NULL_POINTER;
    }

    addr12_t target = decoded_get_immediate(insn);

    // Read current PC
    addr_t current_pc = reg_read_pc(cpu->registers);

    // Calculate return address (next instruction after CALL)
    addr_t return_addr = current_pc + 2;

    // Save return address to S15 (RA register)
    error_t err = reg_write(cpu->registers, REG_RA, return_addr);
    if (err != ERR_OK) {
        return err;
    }

    // Jump to target address
    err = reg_write_pc(cpu->registers, (addr_t)target);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

// ======= RET ======= //
/** Return from subroutine - restore PC from S15 */
error_t exec_ret(CPU_t *cpu, DecodedInstruction_t *insn)
{
    /*
        * [RET:E][unused:12]
        * Return from subroutine
        * Restore PC from S15 (RA register)
        *
        * PC = S15
    */

    if (cpu == NULL || insn == NULL) {
        return ERR_NULL_POINTER;
    }

    // Read return address from S15 (RA register)
    regval_t return_addr;
    error_t err = reg_read(cpu->registers, REG_RA, &return_addr);
    if (err != ERR_OK) {
        return err;
    }

    // Restore PC from return address
    err = reg_write_pc(cpu->registers, return_addr);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}
