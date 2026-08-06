/**
  * @brief Create and allocate CPU with all subsystems
  *
  * Allocates CPU struct and creates RegisterFile and Memory subsystems.
  * Does NOT initialize them (call cpu_init after this).
  *
  * @return Pointer to CPU or NULL on allocation failure
**/

#include "cpu.h"
#include "../registers/registers.h"
#include "../memory/memory.h"
#include "../decoder/decoder.h"
#include "../control/control.h"
#include "../../common/types.h"
#include "../../common/isa_defs.h"
#include "../../utils/errors.h"

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

    cpu->halted = true;         // IDLE (no program loaded)
    cpu->step_mode = false;     // Continuous mode
    cpu->cycle_count = 0;       // Reset cycle counter
    cpu->last_error = ERR_OK;   // Clear any previous errors

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