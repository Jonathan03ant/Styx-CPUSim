#ifndef ISA_IF_H
#define ISA_IF_H

// isa.h — Instruction Opcodes (4-bit ISA)

#define ISA_OP_NOP_I        0x00  
#define ISA_OP_MOV_R        0x01  
#define ISA_OP_LOAD_I       0x02  
#define ISA_OP_STORE_I      0x03  

#define ISA_OP_ADD_R        0x04  
#define ISA_OP_SUB_R        0x05  
#define ISA_OP_MUL_R        0x06  
#define ISA_OP_DIV_R        0x07  
#define ISA_OP_ADDI_I       0x08  
#define ISA_OP_CMP_R        0x09  

#define ISA_OP_JMP_J        0x0A  
#define ISA_OP_JZ_J         0x0B  
#define ISA_OP_JNZ_J        0x0C  
#define ISA_OP_CALL_J       0x0D  
#define ISA_OP_RET_J        0x0E  
#define ISA_OP_IO_I         0x0F  

#define ISA_OP_COUNT        0x10  
#define OPERAND_T_COUNT     3



typedef enum {
    Instruction_t_R,
    Instruction_t_I,
    Instruction_t_J,
}ISA_type;

#endif // ISA_IF_H

