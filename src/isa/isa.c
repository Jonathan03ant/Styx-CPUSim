#include "isa.h"
#include "isa_if.h"
#include "operand.h"

const ISA_t ISA_LUT [ISA_OP_COUNT] = {
    [ISA_OP_NOP_I   ] = {.memonic = "NULL",                     .instruction_type = Instruction_t_I,            .opcode = ISA_OP_NOP_I,             .EXE = EMPTY_CALL_BACK      },
    [ISA_OP_MOV_R   ] = {.memonic = "MOVE_TO_REG",              .instruction_type = Instruction_t_R,            .opcode = ISA_OP_MOV_R,             .EXE = function1            },
    [ISA_OP_LOAD_I  ] = {.memonic = "LOAD_FROM_MEM",            .instruction_type = Instruction_t_I,            .opcode = ISA_OP_LOAD_I,            .EXE = function2            },
    [ISA_OP_STORE_I ] = {.memonic = "STORE_TO_MEM",             .instruction_type = Instruction_t_I,            .opcode = ISA_OP_STORE_I,           .EXE = function3            },
    [ISA_OP_ADD_R   ] = {.memonic = "REG_ADDITION",             .instruction_type = Instruction_t_R,            .opcode = ISA_OP_ADD_R,             .EXE = function4            },                                                             
    [ISA_OP_SUB_R   ] = {.memonic = "REG_SUBTRACTION",          .instruction_type = Instruction_t_R,            .opcode = ISA_OP_SUB_R,             .EXE = function5            },                                                     
    [ISA_OP_MUL_R   ] = {.memonic = "REG_MULTIPLICATION",       .instruction_type = Instruction_t_R,            .opcode = ISA_OP_MUL_R,             .EXE = function6            },                       
    [ISA_OP_DIV_R   ] = {.memonic = "REG_DIVISION",             .instruction_type = Instruction_t_R,            .opcode = ISA_OP_DIV_R,             .EXE = function7            },   
    [ISA_OP_ADDI_I  ] = {.memonic = "ADD_IMM_TO_REG",           .instruction_type = Instruction_t_I,            .opcode = ISA_OP_ADDI_I,            .EXE = function8            },                                                             
    [ISA_OP_CMP_R   ] = {.memonic = "COMPARE_REG_REG",          .instruction_type = Instruction_t_R,            .opcode = ISA_OP_CMP_R,             .EXE = function9            },                                            
    [ISA_OP_JMP_J   ] = {.memonic = "UNCONDITIONAL_JUMP",       .instruction_type = Instruction_t_J,            .opcode = ISA_OP_JMP_J,             .EXE = function10           },                                                   
    [ISA_OP_JZ_J    ] = {.memonic = "JUMP_IF_ZERO",             .instruction_type = Instruction_t_J,            .opcode = ISA_OP_JZ_J,              .EXE = function11           },                                      
    [ISA_OP_JNZ_J   ] = {.memonic = "JUMP_IFN_ZERO",            .instruction_type = Instruction_t_J,            .opcode = ISA_OP_JNZ_J,             .EXE = function12           },                                   
    [ISA_OP_CALL_J  ] = {.memonic = "PUSH_PC_TO_STACK",         .instruction_type = Instruction_t_J,            .opcode = ISA_OP_CALL_J,            .EXE = function13           },                             
    [ISA_OP_RET_J   ] = {.memonic = "POP_PC_FROM_STACK",        .instruction_type = Instruction_t_J,            .opcode = ISA_OP_RET_J,             .EXE = function14           },                         
    [ISA_OP_IO_I    ] = {.memonic = "IO_OPR",                   .instruction_type = Instruction_t_I,            .opcode = ISA_OP_IO_I,              .EXE = function15           },

};

void EMPTY_CALL_BACK() {}