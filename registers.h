#ifndef REGISTERS_H
#define REGISTERS_H

typedef struct {
    int PC;                                      //Program Counter: Stores the address of the next instruction to be FDE'ed
    int ACC;                                     //Accumulator: Stores Arthmetic and Logic Result
    int MAR;                                     //Memory Address Register: Holds Address of Memory to be accessed
    int MDR;                                     //Memory Data Register: Holds data read from or written to memory 
    int CIR;                                     //Current Instruction Register: Holds fetched instruction
    int UTG;                                     //My Own Register :D
} Special_Register;

typedef struct {
    int value;
    int in_use;
} Gen_rgstr;

typedef struct {
    Gen_rgstr R[10];                                   //Array of 10 general registers(R0-R9)
} General_Register;

typedef struct {
    Special_Register isp_rgstr;                          //Special Purpose Registers
    General_Register ign_rgst;                           //General Registers

} Registers;



void init_registers(Registers *regs);
void print_regisrers(const Registers *regs);

int  get_special_registers(const Registers *regs, const char *reg_name);
int  get_general_register(const Registers *regs, int reg_index); 

int  set_special_register( Registers *regs, const char *reg_name, int value);
int  set_general_register(Registers *regs, int reg_index, int value);



#endif //REGISTER_H
