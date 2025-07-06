#ifndef SPECIAL_REG_H
#define SPECIAL_REG_H
#include <stdint.h>


typedef enum 
{
    Reg_PC,                                      //Program Counter: Stores the address of the next instruction to be FDE'ed
    Reg_ACC,                                     //Accumulator: Stores Arthmetic and Logic Result
    Reg_MAR,                                     //Memory Address Register: Holds Address of Memory to be accessed
    Reg_MDR,                                     //Memory Data Register: Holds data read from or written to memory 
    Reg_CIR,                                     //Current Instruction Register: Holds fetched instruction
    Reg_FLAG,                                    //FLAGG REG :D
    Reg_COUNT
} __cpu_Special_Register;


//function to initalzie
//function to get reg value
//function to set reg value
//function to dump current status of regs

void SPE_InitRegs(void);
uint16_t SPE_GetReg(__cpu_Special_Register reg);
void SPE_SetReg(__cpu_Special_Register reg, uint16_t value);
void SPE_DumpRegs(void);

#endif //SPECIAL_REG_H