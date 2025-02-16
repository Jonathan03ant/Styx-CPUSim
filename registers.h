#ifndef REGISTERS_H
#define REGISTERS_H

typedef struct {
    int PC;                                     //Program Counter: Stores the address of the next instruction to be FDE'ed
    int ACC;                                    //Accumulator: Stores Arthmetic and Logic Result
    int MAR;                                    //Memory Address Register: Holds Address of Memory to be accessed
    int MDR;                                    //Memory Data Register: Holds data read from or written to memory 
    int CIR;                                    //Current Instruction Register: Holds fetched instruction
    int UTG;                                    //My Own Register :D
} sp_rgstr;

typedef struct {
    int R[10];                                  //Array of 10 general registers(R0-R9)
} gn_rgstr;

typedef struct {
    sp_rgstr isp_rgstr;                         //Special Purpose Registers
    gn_rgst ign_rgst;                           //General Registers

} rgstr;
#endif //REGISTER_H
