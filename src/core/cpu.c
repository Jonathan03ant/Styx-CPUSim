/**
  * @brief Create and allocate CPU with all subsystems
  *
  * Allocates CPU struct and creates RegisterFile and Memory subsystems.
  * Does NOT initialize them (call cpu_init after this).
  *
  * @return Pointer to CPU or NULL on allocation failure
**/

#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "decoder.h"
#include "control.h"
#include "common/types.h"
#include "common/isa_defs.h"
#include "util/errors.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

//* Allocates (Not Init) CPU struct and creates RegisterFile and Memory subsystems.
CPU_t *cpu_create(void)
{
    /*
        * Allocate CPU struct
        * Zero everything
    */

    CPU_t* cpu = (CPU_t*)malloc(sizeof(CPU_t));
    if (cpu == NULL) {
        return NULL;
    }

    memset(cpu, 0, sizeof(CPU_t));

    // Create CPU subsystems
    cpu->registers  = reg_create();
    if (cpu->registers == NULL) {
        free(cpu);
        return NULL;
    }

    cpu->memory     = mem_create();
    if (cpu->memory == NULL) {
        reg_destroy(cpu->registers);
        free(cpu);
        return NULL;
    }

    return cpu;
}

//* Initializes all subsystems (RegisterFile, Memory) to their reset values.
error_t cpu_init(CPU_t *cpu)
{
    /*
        * TO be called after cpu_create() and used as soft reset
        * Initializes all sub systems and sets CPU to idle state
    */

    if (cpu == NULL) {
        return ERR_NULL_POINTER;
    }

    // Initialize RegisterFile and Memory
    // After this: S0-S12=0, S13(SP)=0xFFFF, S14-S15=0, PC=0x0000, FLAGS=0x0000
    //             All 64kb Zeroes, protection Mode = loading
    reg_init(cpu->registers);
    mem_init(cpu->memory);

    cpu->MAR        = 0x0000;
    cpu->MDR        = 0x0000;
    cpu->IR         = 0x0000;

    cpu->state = CPU_STATE_IDLE;         // IDLE (no program loaded)
    cpu->step_mode = false;              // Continuous mode
    cpu->cycle_count = 0;                // Reset cycle counter
    cpu->last_error = ERR_OK;            // Clear any previous errors

    return ERR_OK;
}

//*  Destroys all subsystems (RegisterFile, Memory) and frees CPU struct.
void cpu_destroy(CPU_t *cpu)
{
    /*
        * Destroy register file subsystem
        * Destroy memory subsystem
        * Free CPU struct itself
    */

    if (cpu == NULL) {
        return; //safe to call on null
    }

    if (cpu->registers != NULL) {
        reg_destroy(cpu->registers);
        cpu->registers = NULL;
    }

    if (cpu->memory != NULL) {
        mem_destroy(cpu->memory);
        cpu->memory = NULL;
    }

    free(cpu);
}

//* Reset CPU to IDLE state (without destroying/recreating)
void cpu_reset(CPU_t *cpu)
{
    /*
        * Resets CPU back to IDLE state
        * Clears all registers, memory, and execution state
        * Can be called after program execution to start fresh
    */

    if (cpu == NULL) {
        return;
    }

    // Reset RegisterFile and Memory to initial state
    reg_reset(cpu->registers);
    mem_reset(cpu->memory);

    // Clear internal registers
    cpu->MAR        = 0x0000;
    cpu->MDR        = 0x0000;
    cpu->IR         = 0x0000;

    // Reset execution state to IDLE
    cpu->state      = CPU_STATE_IDLE  ;         // IDLE (no program loaded)
    cpu->step_mode  = false;                    // Continuous mode
    cpu->cycle_count = 0;                       // Reset cycle counter
    cpu->last_error = ERR_OK;                   // Clear any previous errors
}

//* Soft reset - restart loaded program without wiping memory
void cpu_soft_reset(CPU_t *cpu)
{
    /*
     * Soft reset - restart loaded program from entry point
     * Resets PC to 0x0100, clears registers/FLAGS
     * Memory and loaded program remain intact
     * State remains LOADED (not IDLE)
     */

    if (cpu == NULL) {
        return;
    }

    // Only valid if program is loaded
    if (cpu->state != CPU_STATE_LOADED &&
        cpu->state != CPU_STATE_RUNNING &&
        cpu->state != CPU_STATE_HALTED) {
        return;
    }

    // Reset registers (clears all to 0, sets SP to 0xFFFF)
    reg_reset(cpu->registers);

    // Override PC to entry point (0x0100, not 0x0000)
    reg_write_pc(cpu->registers, 0x0100);

    // Clear internal registers
    cpu->MAR = 0x0000;
    cpu->MDR = 0x0000;
    cpu->IR = 0x0000;

    // Reset execution state
    cpu->cycle_count = 0;
    cpu->last_error = ERR_OK;

    // Keep state as LOADED (program still in memory!)
    cpu->state = CPU_STATE_LOADED;
}

//* Manually halt CPU execution
void cpu_halt(CPU_t *cpu)
{
    /*
        * Stops execution in cpu_run() loop
    */

    if (cpu == NULL) {
        return;
    }

    if (cpu->state == CPU_STATE_RUNNING) {
        cpu->state = CPU_STATE_HALTED;  // ← CHANGE from halted = true
    }
}


//* Get total number of instructions executed
uint32_t cpu_get_cycle_count(CPU_t *cpu)
{
    /*
        * Returns cycle count (instructions executed)
        * Returns 0 if cpu is NULL
    */

    if (cpu == NULL) {
        return 0;
    }

    return cpu->cycle_count;
}

//* Reset cycle counter and statistics
void cpu_reset_stats(CPU_t *cpu)
{
    /*
        * Clears cycle count and last error
        * Does NOT reset registers/memory (use cpu_reset for that)
    */

    if (cpu == NULL) {
        return;
    }

    cpu->cycle_count = 0;
    cpu->last_error = ERR_OK;
}


//* Execute one instruction (fetch-decode-execute cycle)
error_t cpu_step(CPU_t *cpu)
{
    /*
       * Executes a single instruction (one FDE cycle)
       * Transitions LOADED → RUNNING on first step
       * Detects HALT pattern (JMP to self)
       * On error: sets CPU_STATE_ERROR and last_error
    */

    if (cpu == NULL) {
        return ERR_NULL_POINTER;
    }

    // Can only step is LOADED or RUNNING
    if (cpu->state != CPU_STATE_LOADED &&
        cpu->state != CPU_STATE_RUNNING){
        return ERR_INVALID_CPU_STATE;
    }

    // Transition LOADED->RUNNING on first step
    if (cpu->state == CPU_STATE_LOADED){
        cpu->state = CPU_STATE_RUNNING;
    }

    // FETCH, GET pc and read instruction from memory
    addr_t pc = reg_read_pc(cpu->registers);
    insn_t raw_instruction;
    error_t err = ctrl_fetch(cpu->memory, pc, &raw_instruction);
    if (err != ERR_OK) {
        cpu->state = CPU_STATE_ERROR;
        cpu->last_error = err;
        return err;
    }

    // Store IR
    cpu->IR = raw_instruction;

    // Decode
    DecodedInstruction_t decoded;
    err = decode_instruction(raw_instruction, &decoded);
    if (err != ERR_OK) {
        cpu->state = CPU_STATE_ERROR;
        cpu->last_error = err;
        return err;
    }

    // Execute  (ctrl_execute auto-increments PC)
    err = ctrl_execute(cpu, &decoded);
    if (err != ERR_OK) {
        cpu->state = CPU_STATE_ERROR;
        cpu->last_error = err;
        return err;
    }

    // Update stats
    cpu->cycle_count++;

    // Check for HALT pattern (JMP to self)
    // NOTE: Must check BEFORE execute, but we already executed!
    // So check if we jumped to the CURRENT instruction (PC before increment was 'pc')
    if (decoded.opcode == OP_JMP) {
        addr_t current_pc = reg_read_pc(cpu->registers);  // PC after JMP
        addr_t jump_target = decoded.fields.j_type.addr;
        // Check if we jumped to the same address we just executed from
        if (jump_target == pc) {  // 'pc' was captured before execute
            cpu->state = CPU_STATE_HALTED;
        }
    }

    return ERR_OK;
}

//* Run CPU until HALTED or ERROR
error_t cpu_run(CPU_t *cpu)
{
    /*
        * Executes instructions continuously until:
        * - HALT pattern detected (JMP to self)
        * - Error occurs
        * Each iteration calls cpu_step()
    */

    if (cpu == NULL) {
        return ERR_NULL_POINTER;
    }

    // Can only run if LOADED or RUNNING
    if (cpu->state != CPU_STATE_LOADED &&
        cpu->state != CPU_STATE_RUNNING) {
        return ERR_INVALID_CPU_STATE;
    }

    // Keep stepping until halted or error
    while(cpu->state == CPU_STATE_LOADED ||
          cpu->state == CPU_STATE_RUNNING){
        error_t err = cpu_step(cpu);
        if (err != ERR_OK) {
            return err;  // Error already set by cpu_step
        }
    }

    return ERR_OK;
}

//* Run CPU for N instructions
error_t cpu_run_for(CPU_t *cpu, uint32_t n)
{
    /*
       * Executes exactly N instructions (or until HALT/ERROR)
       * Useful for:
       * - Testing (run known number of steps)
       * - Debugging (step through slowly)
       * - Performance testing
    */

    if (cpu == NULL) {
        return ERR_NULL_POINTER;
    }

    // Can only run if LOADED or RUNNING
    if (cpu->state != CPU_STATE_LOADED &&
        cpu->state != CPU_STATE_RUNNING) {
        return ERR_INVALID_CPU_STATE;
    }

    // Execute N instructions
    for (uint32_t i = 0; i < n; i++){
        if (cpu->state != CPU_STATE_LOADED &&
        cpu->state != CPU_STATE_RUNNING) {
            break;
        }
        error_t err = cpu_step(cpu);
        if (err != ERR_OK) {
            return err;
        }
    }
    return ERR_OK;
}