#include "special_reg.h"

static uint16_t SPE_Registers[Reg_COUNT];

void SPE_initRegs(void)
{
    for (int i = 0; i<=Reg_COUNT; i++)
    {
        SPE_Registers[i] = 0;
    }
}