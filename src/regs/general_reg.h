#ifndef GENERAL_REG_H
#define GENERAL_REG_H
#include "stdint.h"

typedef enum 
{
    REG_Gen_R0,
    REG_Gen_R1,
    REG_Gen_R2,
    REG_Gen_R3,
    REG_Gen_R4,
    REG_Gen_R5,
    REG_Gen_R6,
    REG_Gen_R7,
    REG_Gen_R8,
    REG_Gen_R9,
    REG_Gen_R10,
    REG_Gen_R11,
    REG_Gen_R11,
    REG_Gen_R12,
    REG_Gen_R13,
    REG_Gen_R14,
    REG_Gen_R15,
    REG_COUNT_GEN
}__cpu_General_Register;


void GEN_InitRegs(void);
uint16_t GEN_GetRegs(__cpu_General_Register regs);
void GEN_SetRegs(__cpu_General_Register regs, uint16_t value);
void GEN_DumpRegs(void);








#endif