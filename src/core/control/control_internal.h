/**
  * @file control_internal.h
  * @brief Control Unit - Internal Handler Declarations
  *
  * This header is shared between control.c and all exec_*.c files.
  * It declares all instruction handler functions.
  *
  * NOT part of the public API - only included by control unit files.
 */

#ifndef CONTROL_INTERNAL_H
#define CONTROL_INTERNAL_H

#include "control.h"

//* ARITHMETIC HANDLERS (exec_arith.c) //
error_t exec_add(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_sub(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_mul(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_div(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_addi(CPU_t *cpu, DecodedInstruction_t *insn);

//* MEMORY HANDLERS (exec_memory.c) //
error_t exec_load(CPU_t* cpu, DecodedInstruction_t* insn);
error_t exec_store(CPU_t* cpu, DecodedInstruction_t* insn);

//* CONTROL HANDLERS (exec_control.c) //
error_t exec_jmp(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_js(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_jnz(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_call(CPU_t *cpu, DecodedInstruction_t *insn);
error_t exec_ret(CPU_t *cpu, DecodedInstruction_t *insn);

//* MISC HANDLERS (exec_misc.c) //
error_t exec_mov(CPU_t* cpu, DecodedInstruction_t* insn);
error_t exec_cmp(CPU_t* cpu, DecodedInstruction_t* insn);
error_t exec_lui(CPU_t* cpu, DecodedInstruction_t* insn);
error_t exec_nop(CPU_t* cpu, DecodedInstruction_t* insn);

#endif // CONTROL_INTERNAL_H