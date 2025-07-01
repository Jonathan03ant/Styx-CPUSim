#ifndef REGISTERS_H
#define REGISTERS_H
#include "stdbool.h"


typedef enum {
    Reg_PC,                                      //Program Counter: Stores the address of the next instruction to be FDE'ed
    Reg_ACC,                                     //Accumulator: Stores Arthmetic and Logic Result
    Reg_MAR,                                     //Memory Address Register: Holds Address of Memory to be accessed
    Reg_MDR,                                     //Memory Data Register: Holds data read from or written to memory 
    Reg_CIR,                                     //Current Instruction Register: Holds fetched instruction
    Reg_FLAG                                     //FLAGG REG :D
} __cpu_Special_Register;

typedef struct {
    int value;
    bool free;
} __cpu_Gen_rgstr;

typedef struct {
    __cpu_Gen_rgstr R[10];                                   //Array of 10 general registers(R0-R9)
} __cpu_General_Register;

typedef struct {
    __cpu_Special_Register _cpu_sp_rgstr;                          //Special Purpose Registers
    __cpu_General_Register _cpu_gn_rgstr;                           //General Registers

} Registers;



void init_registers(Registers *regs);
void print_registers(const Registers *regs);

int  get_cpu_special_registers(const Registers *regs, const char *reg_name);
int  get_cpu_general_register(const Registers *regs, int reg_index); 

int  set_cpu_special_register(Registers *regs, const char *reg_name, int value);
int  set_cpu_general_register(Registers *regs, int reg_index, int value);

/*
function to find a gen register not in use
*/

#endif //REGISTER_H
