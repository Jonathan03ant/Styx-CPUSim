/**
  * @file control.c
  * @brief Control Unit - Main Orchestrator
  *
  * Implements fetch-decode-execute orchestration:
  * - FETCH: Read instruction from memory
  * - EXECUTE: Dispatch to handlers via function pointer table
 */

#include "control.h"
#include "../handlers/isa_handlers.h"
#include "decoder.h"
#include "common/isa_defs.h"
#include <stddef.h>

// *Dispatch Table //
/**
 * Instruction handler displatch table
 * indexed by opcode
 */
static InstructionHandler_t handler_table[OP_COUNT] = {
    [OP_NOP]   = exec_nop,      // 0x0
    [OP_MOV]   = exec_mov,      // 0x1
    [OP_LOAD]  = exec_load,     // 0x2
    [OP_STORE] = exec_store,    // 0x3
    [OP_ADD]   = exec_add,      // 0x4
    [OP_SUB]   = exec_sub,      // 0x5
    [OP_MUL]   = exec_mul,      // 0x6
    [OP_DIV]   = exec_div,      // 0x7
    [OP_JMP]   = exec_jmp,      // 0x8
    [OP_JZ]    = exec_jz,       // 0x9
    [OP_JNZ]   = exec_jnz,      // 0xA
    [OP_CMP]   = exec_cmp,      // 0xB
    [OP_CALL]  = exec_call,     // 0xC
    [OP_RET]   = exec_ret,      // 0xD
    [OP_ADDI]  = exec_addi,     // 0xE
    [OP_LUI]   = exec_lui,      // 0xF
};

// *Control Fetch, Read 16-bit word from memory at a given address //
error_t ctrl_fetch(Memory_t *mem, addr_t mem_addr, insn_t *instruction)
{
    if (mem == NULL || instruction == NULL){
        return ERR_NULL_POINTER;
    }

    //*Read 16-bit instruction from memory at PC
    word_t raw;
    error_t err = mem_read_word(mem, mem_addr, &raw);
    if (err != ERR_OK){
        return err;
    }
    //*Store fetched instruction
    *instruction = (insn_t)raw;

    return ERR_OK;
}

// *EXE //
error_t ctrl_execute(CPU_t *cpu, DecodedInstruction_t *decoded){
    if (cpu == NULL || decoded == NULL){
        return ERR_NULL_POINTER;
    }

    if (!is_valid_opcode(decoded->opcode)){
        return ERR_INVALID_OPCODE;
    }

    //*Get handler
    InstructionHandler_t handler = handler_table[decoded->opcode];
    if (handler == NULL){
        return ERR_INVALID_OPCODE;
    }
    //*Call the handler
    error_t err = handler(cpu, decoded);
    if (err != ERR_OK){
        return err;
    }

    if (!instruction_is_ctl_flow(decoded->opcode)) {
        reg_increment_pc(cpu->registers);
    }

    return ERR_OK;
}
