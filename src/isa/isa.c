#include "isa.h"
#include "isa_if.h"
#include "operand.h"

extern const ISA_defn ISA_LUT[ISA_OP_COUNT] = {
    [ISA_OP_NOP_I]      = {
        
    },
    [ISA_OP_MOV_R]      = {

    },
    [ISA_OP_LOAD_I]     = {

    },
    [ISA_OP_STORE_I]    = {

    },
    [ISA_OP_ADD_R]      = {

    },
    [ISA_OP_SUB_R]      = {

    },
    [ISA_OP_MUL_R]      = {

    },
    [ISA_OP_DIV_R]      = {

    },
    [ISA_OP_ADDI_I]     = {

    },
    [ISA_OP_CMP_R]      = {

    },
    [ISA_OP_JMP_J]      = {

    },
    [ISA_OP_JZ_J]       = {

    },
    [ISA_OP_JNZ_J]      = {

    },
    [ISA_OP_CALL_J]     = {

    },
    [ISA_OP_RET_J]      = {

    },
    [ISA_OP_IO_I]       = {

    },

};