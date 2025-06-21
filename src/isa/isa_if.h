#ifndef ISA_IF_H
#define ISA_IF_H

// isa.h — Instruction Opcodes (4-bit ISA)

#define ISA_OP_NOP_I        0x00  ///< No Operation (I-type)
#define ISA_OP_MOV_R        0x01  ///< Move register-to-register (R-type)
#define ISA_OP_LOAD_I       0x02  ///< Load from memory into register (I-type)
#define ISA_OP_STORE_I      0x03  ///< Store register to memory (I-type)

#define ISA_OP_ADD_R        0x04  ///< Integer addition (R-type)
#define ISA_OP_SUB_R        0x05  ///< Integer subtraction (R-type)
#define ISA_OP_MUL_R        0x06  ///< Integer multiplication (R-type)
#define ISA_OP_DIV_R        0x07  ///< Integer division (R-type)

#define ISA_OP_ADDI_I       0x08  ///< Add immediate to register (I-type)
#define ISA_OP_CMP_R        0x09  ///< Compare two registers (R-type), set flags

#define ISA_OP_JMP_J        0x0A  ///< Unconditional jump (J-type)
#define ISA_OP_JZ_J         0x0B  ///< Jump if zero flag set (J-type)
#define ISA_OP_JNZ_J        0x0C  ///< Jump if zero flag clear (J-type)
#define ISA_OP_CALL_J       0x0D  ///< Call subroutine (J-type)
#define ISA_OP_RET_J        0x0E  ///< Return from subroutine (J-type)

#define ISA_OP_IO_I         0x0F  ///< Input/output operation (I-type, shared IN/OUT)

#define ISA_OP_COUNT        0x10  ///< Total number of defined opcodes (16 total)

#define OPERAND_T_COUNT     3



typedef enum {
    Instruction_t_R,
    Instruction_t_I,
    Instruction_t_J,
}ISA_type;

#endif // ISA_IF_H

