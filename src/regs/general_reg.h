#ifndef GENERAL_REG_H
#define GENERAL_REG_H
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    General Registers ID Enumeration (16 registers)
    * From 0000 to 1111 
    * Each register[i] is 16 bit addressable 
*/
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
    REG_Gen_R12,
    REG_Gen_R13,
    REG_Gen_R14,
    REG_Gen_R15,
    REG_COUNT_GEN
}__cpu_General_Register;


/*
    Register Context Snapshot
*/
typedef struct {
    uint16_t regs[REG_COUNT_GEN];
}GenRegister_ctx; //Register context snapshot


void GEN_InitRegs(void); //Reset
bool Is_Valid(__cpu_General_Register reg); //Bound Check

//Register Access API (CPU-ALU Facing)
uint16_t GEN_GetReg(__cpu_General_Register reg);
void GEN_SetReg(__cpu_General_Register reg, uint16_t value);
uint16_t *RegPtr(void); // RAW pointer to raw register array

// Register Context Save/Restore
void GEN_SaveContext(GenRegister_ctx *context); //Save full register state/context
void GEN_LoadContext(const GenRegister_ctx *context); //Restore register state/context from ctx


bool GEN_ParseRegs(uint8_t regindex, __cpu_General_Register *output); // given 4 bit register address (0xF), retrun the enum register or false

void GEN_DumpRegs(void);


#ifdef __cplusplus
}
#endif

#endif // GENERAL_REG_H