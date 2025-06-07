// isa.h
#ifndef ISA_H
#define ISA_H

#include <stdint.h>
#include <isa_if.h>
#include <operand.h>


typedef enum {
    Instruction_t_I,
    Instruction_t_J,
    Instruction_t_R
}Instruction_t;


//statically defined instructions for all 16 of them 
typedef struct {
    const char *memonic; 
    uint8_t opcode;   
    Instruction instruction_type;
    void *EXE(void *operand);         // run time isa exe function pointer
} ISA_db;

extern const ISA_db ISA_table[ISA_OP_COUNT];

typedef struct {
    uint8_t opcode;
    union {
        OperandR r;
        OperandI i;
        OperandJ j;
    } operand;
    const ISA_db *entry;  // points to isa_table[opcode]
} Instruction;


#endif //isa.h
