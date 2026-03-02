#include <stdlib.h>
#include <string.h>
#include "registers.h"
#include "../../common/reg_defs.h"
#include "../../utils/errors.h"


/*----------------------------------------------------------------------------
  * REGISTER FILE STRUCT DEFINITION
  *----------------------------------------------------------------------------*/
struct RegisterFile_s {
    regval_t gpr[REG_COUNT];            // General-purpose registers S0-S15
    addr_t pc;                          // Program counter
    flags_t flags;                      // Status flags (Z, N, C, V)
};

/*
    * Create RegisterFile struct (one time)
    * Referenced by RegisterFile_t, calls ref_init()
*/
RegisterFile_t *reg_create(void)
{
    RegisterFile_t *rf = malloc(sizeof(RegisterFile_t));\
    if (rf == NULL){
        return NULL;
    }
    reg_init(rf);
    return rf;
}

/*
    * Initializes RegisterFile_t
*/
void reg_init(RegisterFile_t *rf)
{
    if (rf == NULL) return;

    memset(rf->gpr, 0, sizeof(rf->gpr));
    // Initial values
    rf->gpr[REG_SP] = REG_INIT_SP;      // S13 SP = 0xFFFF
    rf->pc          = REG_INIT_PC;      // PC = 0x0000
    rf->flags       = REG_INIT_FLAGS;   // FLAGS = 0x0000
}

/*
    * Free RegisterFile_t
*/
void reg_destroy(RegisterFile_t *rf)
{
    if (rf != NULL){
        free(rf);
    }
}

/*
    * Reset RegisterFile_t to initial values
    * Alis of reg_init()
*/
void reg_reset(RegisterFile_t *rf)
{
    reg_init(rf);
}

