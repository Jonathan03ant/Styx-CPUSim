/**
  * @file control.h
  * @brief Control Unit - Fetch-Decode-Execute Orchestration
  *
  * The Control Unit orchestrates instruction execution:
  * - FETCH: Read instruction from memory at PC
  * - EXECUTE: Dispatch to instruction handlers based on opcode
  *
  * This module ties together the decoder, ALU, registers, and memory
  * to implement the CPU's fetch-decode-execute cycle.
 */

#ifndef CONTROL_H
#define CONTROL_H

#include "cpu.h"
#include "decoder.h"
#include "memory.h"
#include "common/types.h"
#include "util/errors.h"

/**
  * *Fetch instruction from memory
  *
  * Reads a16-bit instruction from memory at a given address ([pc])
  * This is the fetch phase of the fetch-decode-execute cycle
  *
  * @param mem Memory module contains 64KB memory array
  * @param addr Any gven, address to fetch from memory
      * CPU could use this to fetch at address PC
  * @param instruction output: Fetched 16-bit instruction
 */
error_t ctrl_fetch(Memory_t *mem, addr_t mem_addr, insn_t *instruction);

/**
  * *Executes a decoded instruction
  *
  * Dispatches to the appropriate instruction handler based on opcode
  * Handlers update CPU state (Registers, memory, PC, FLAGS
  *
  * This is the Execute phase of the fetch-decode-execute cycle
  *
  * @param cpu CPU State (registers, memory, internal registers)
  * @param decoded Decoded instruction (opcode, format, oprands)
 */
error_t ctrl_execute(CPU_t *cpu, DecodedInstruction_t *decoded);

/**
  * *Instruction handler function pointer type
  *
  * All instruction handlers have this signature
  * - Input: CPU state + decoded instruciton
  * - Output: Error code
  * Handlers are responsible for
  * 1. Extracting operands from decoded instruction
  * 2. Read register values
  * 3. Perform operations (ALU/MEM/Register transfer)
  * 4. Write results back to register, update FLAGS
  * 5. Update MAR/MDR and increment PC
  *
  * Non-control-flow handlers do NOT increment PC - ctrl_execute does it automatically.
  * Control-flow handlers set PC directly (JMP, JZ, JNZ, CALL, RET).
 */
typedef error_t (*InstructionHandler_t)(CPU_t *cpu, DecodedInstruction_t *insn);

#endif // CONTROL_H