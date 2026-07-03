/**
  * @file cpu.h
  * @brief CPU - TOP-Level orchestrator
  *
  * The CPU module ties together all components (Registers, memory, decoder, ALU)
  * and provides the main execution interface (step, run, reset)
  */

#ifndef CPU_H
#define CPU_H

#include "../registers/registers.h"
#include "../memory/memory.h"
#include "../../common/types.h"
#include "../../utils/errors.h"

#include <stdbool.h>
#include <stdint.h>

/**
  * * CPU State
  * Contains all components and Internal States needed for execution
  * Internal registers (MAR/MDR/IR) are not ISA-visible but provide
  * * debugging visibility into the fetch-decode-execute cycle.
**/

typedef struct CPU_s {
    //*Core Components//
    RegisterFile_t *registers;
    Memory_t       *memory;

    //*Internal Registers//
    addr_t MAR;                 // Memory Adress Register
    addr_t MDR;                 // Memory Data Register
    insn_t IR;                  // Instruction Register (Current Instruction)

    //*Execution State//
    bool halted;                // CPU Stopped? (HALT Instruction or error)
    bool step_mode;             // true = single step, false = continuous
    uint32_t cycle_count;       // total instruction executed
    error_t last_error;
} CPU_t;

#endif // CPU_H