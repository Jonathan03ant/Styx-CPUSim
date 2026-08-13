/**
  * @file cpu.h
  * @brief CPU - TOP-Level orchestrator
  *
  * The CPU module ties together all components (Registers, memory, decoder, ALU)
  * and provides the main execution interface (step, run, reset)
  */

#ifndef CPU_H
#define CPU_H

#include "registers.h"
#include "memory.h"
#include "common/types.h"
#include "util/errors.h"

#include <stdbool.h>
#include <stdint.h>

/*
  * CPU Power state, important for execution and TUI
*/
typedef enum {
    CPU_STATE_IDLE,               // Powered on, no program
    CPU_STATE_LOADED,             // Program loaded, ready to execute
    CPU_STATE_RUNNING,            // Executing instructions
    CPU_STATE_HALTED,             // Execution finished (HALT)
    CPU_STATE_ERROR               // Stopped due to error
} CPUState_e;



/**
  * * CPU Struct
  * Contains all components and Internal States needed for execution
  * Internal registers (MAR/MDR/IR) are not ISA-visible but provide
  * * debugging visibility into the fetch-decode-execute cycle.
**/
typedef struct CPU_s {
    //*Core Components//
    RegisterFile_t *registers;
    Memory_t       *memory;
    CPUState_e      state;

    //*Internal Registers//
    addr_t MAR;                 // Memory Adress Register
    word_t MDR;                 // Memory Data Register
    insn_t IR;                  // Instruction Register (Current Instruction)

    //*Execution State//
    bool step_mode;             // true = single step, false = continuous
    uint32_t cycle_count;       // total instruction executed
    error_t last_error;
} CPU_t;

CPU_t *cpu_create(void);
error_t cpu_init(CPU_t *cpu);
void cpu_destroy(CPU_t *cpu);

error_t cpu_step(CPU_t *cpu);
error_t cpu_run(CPU_t *cpu);
error_t cpu_run_for(CPU_t *cpu, uint32_t n);

void cpu_reset(CPU_t *cpu);
void cpu_halt(CPU_t *cpu);

void cpu_print_state(CPU_t* cpu);
uint32_t cpu_get_cycle_count(CPU_t* cpu);
void cpu_reset_stats(CPU_t* cpu);


#endif // CPU_H