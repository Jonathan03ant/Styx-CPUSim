#ifndef STYX_REGISTERS_H
#define STYX_REGISTERS_H

#include "../../common/types.h"
#include "../../common/reg_defs.h"
#include "../../utils/errors.h"

/*----------------------------------------------------------------------------
  * REGISTER FILE = PUBLIC API
  *----------------------------------------------------------------------------*/
typedef struct RegisterFile_s RegisterFile_t;

RegisterFile_t *reg_create(void); // crate register file
void reg_init(RegisterFile_t *rf);
void reg_destroy(RegisterFile_t *rf);
void reg_reset(RegisterFile_t *rf);


// GENERAL PURPOSE REGISTRATION OPS (S0-S15)
// read and write enforces S0 and validate reg_num
error_t reg_read(const RegisterFile_t *rf, reg_t reg_num, regval_t *value);
error_t reg_write(RegisterFile_t *rf, reg_t reg_num, regval_t value);

regval_t reg_read_pc(const RegisterFile_t *rf);                                                   // Read PC
error_t  reg_write_pc(RegisterFile_t *rf, addr_t new_pc);                                   // Write new PC, in case of jump
void     reg_increment_pc(RegisterFile_t *rf);

// FLAGS REGISTER OPERATIONS
flags_t reg_get_flags(const RegisterFile_t *rf);                                            // Get Flag Reg vale
void    reg_set_flags(RegisterFile_t *rf, flags_t flags);                                   // Used by CMP, setting
flag_bit_t reg_read_flag_bit(const RegisterFile_t *rf, flags_t flag_mask);                 // Read individual bits (ZNCV)
void    reg_set_flag_bit(RegisterFile_t *rf, flags_t  flag_mask, flag_bit_t value);
void    reg_clear_flags(RegisterFile_t *rf);

// DEBUGGING
static inline bool reg_is_valid_num(reg_t reg_num)
{
    return reg_num < REG_COUNT;
}

static inline bool reg_is_zero_reg(reg_t reg_num)
{
      return reg_num == REG_S0;
}

void reg_debug_print(const RegisterFile_t *rf);






#endif /* STYX_REGISTERS_H */