// isa.h
#ifndef ISA_H
#define ISA_H

#include <stdint.h>
#include <isa_if.h>
#include <operand.h>


typedef enum {
    ISA_type_I,
    ISA_type_J,
    ISA_type_R
}ISA_type;

typedef struct {
    const char *memonic; 
    uint8_t opcode;   
    ISA_type instruction_type;
    void *OPC_handler(void *operand);         // run time isa exe function pointer
} ISA_entry;

extern const ISA_entry ISA_table[ISA_OP_COUNT];

typedef struct {
    uint8_t opcode;
    union {
        OperandR r;
        OperandI i;
        OperandJ j;
    } operand;
    const ISA_entry *entry;  // points to isa_table[opcode]
} Instruction;


#endif //isa.h
