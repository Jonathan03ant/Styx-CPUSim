// isa.h
#ifndef ISA_H
#define ISA_H

#include <stdint.h>
#include <isa_if.h>
#include <operand.h>

//statically defined instructions for all 16 of them 
typedef struct iSA_dbs {
    const char *memonic; 
    uint8_t opcode;   
    ISA_type instruction_type;
    void (*EXE)(void *operand);         // run time isa exe function pointer
}ISA_dbs;

extern const ISA_dbs ISA_table[ISA_OP_COUNT];

typedef struct {
    uint8_t opcode;
    union {
        OperandR r;
        OperandI i;
        OperandJ j;
    } operand;
    const ISA_dbs *entry;  // points to isa_table[opcode]
}ISA_rt;


#endif //isa.h
