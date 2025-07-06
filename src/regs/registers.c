#include "special_reg.h"

static uint16_t SPE_Registers[Reg_COUNT];
static uint16_t GEN_Registers[REG_COUNT_GEN]

void SPE_initRegs(void)
{
    for (int i = 0; i < Reg_COUNT; i++)
    {
        SPE_Registers[i] = 0;
    }
}

uint16_t SPE_GetReg(__cpu_Special_Register reg)
{
    if (reg >= Reg_COUNT)
    {
        return 0xFFF; // error handling maybe implemented later
    }

    return SPE_Registers[reg];
}

void SPE_SetReg(__cpu_Special_Register reg, uint16_t value)
{
    if (reg >= Reg_COUNT)
    {
        return; 
    }
    
    SPE_Registers[reg] = value;
}