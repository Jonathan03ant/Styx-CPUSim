#include "special_reg.h"
#include "general_reg.h"
#include "../error.h"
#include <string.h>

// Live General Register File (16 X 16-bit)
static GenRegister_ctx General_Registers;

static inline bool reg_valid(cpu_General_Register reg) 
{
    return reg < REG_COUNT_GEN;
}

void Gen_InitReg(void)
{
    memset(&General_Registers, 0, sizeof(General_Registers));
}


bool IS_Valid(cpu_General_Register reg)
{
    return reg_valid(reg);
}


uint16_t GEN_GetReg(cpu_General_Register reg)
{
    if (!reg_valid(reg)) {
        RAISE_ERROR(ERR_INVALID_REGISTER, "GEN_GetReg: invalid general register id");
        return 0; 
    }
    return General_Registers.regs[reg];
}

void GEN_SetReg(cpu_General_Register reg, uint16_t value)
{
    if (!reg_valid(reg)) {
        RAISE_ERROR(ERR_INVALID_REGISTER, "GEN_SetReg: invalid general register id");
        return;
    }
    General_Registers.regs[reg] = value;
}

uint16_t *RegPtr(void)
{
    return General_Registers.regs;
}