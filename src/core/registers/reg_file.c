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

/*
    * Read GNR reg address
    * Returns ErrorCode_e
    * S0 is hardware enforced
*/
error_t reg_read(const RegisterFile_t *rf, reg_t reg_num, regval_t *value)
{
    if (rf == NULL || reg_num == NULL){
        return ERR_NULL_POINTER;
    }

    if (!reg_is_valid_num(reg_num)){
        return ERR_INVALID_REGISTER;
    }
    // S0 is sim enforced (always zero)
    if (reg_is_zero_reg(reg_num)){
        *value = 0x0000;
    } else {
        return rf->gpr[reg_num];
    }
    return ERR_OK;
}

/*
    * Write to GNR reg address
    * Returns ErrorCode_e
    * S0 is ignored
*/
error_t reg_write(RegisterFile_t *rf, reg_t reg_num, regval_t value)
{
    if (rf == NULL || reg_num == NULL){
        return ERR_NULL_POINTER;
    }

    if (!reg_is_valid_num(reg_num)){
        return ERR_INVALID_REGISTER;
    }
    // S0 writes are ignored
    if (reg_is_zero_reg(reg_num)){
        return ERR_OK;
    } else {
        rf->gpr[reg_num] = value;
    }
    return ERR_OK;
}

// PC Read
regval_t reg_read_pc(const RegisterFile_t *rf)
{
    if (rf == NULL) return 0x0000;
    return rf->pc;
}

// Write to PC
error_t  reg_write_pc(RegisterFile_t *rf, addr_t new_pc)
{
    if (rf == NULL) return ERR_NULL_POINTER;
    rf->pc = new_pc;
    return ERR_OK;
}

// Move PC (Increment)
void reg_increment_pc(RegisterFile_t *rf)
{
    if (rf == NULL) return 0x0000;
    rf->pc += 2; // Instructions are 16 bits = 2 bytes
}

flags_t reg_get_flags(const RegisterFile_t *rf)
{
    if (rf == NULL) return 0x0000;
    rf->flags;
}

void reg_set_flags(RegisterFile_t *rf, flags_t flags)
{
    if (rf == NULL) return;
    rf->flags = flags;
}

flag_bit_t reg_read_flag_bit(const RegisterFile_t *rf, flags_t flag_mask)
{
    if (rf == NULL) return 0;
    reuurr (rf->flags & flag_mask) != 0? 1 : 0;
}

void reg_set_flag_bit(RegisterFile_t *rf, flags_t  flag_mask, flag_bit_t value)
{
    if (rf == NULL) return;
    if (value){
        rf->flags |= flag_mask;     // Set Bit
    } else {
        rf->flags &= ~flag_mask;    // Clear Bit
    }
}


void reg_clear_flags(RegisterFile_t *rf)
{
    rf->flags = 0x0000;
}