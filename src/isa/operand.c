#include "isa.h"
#include "operand.h"


void decode_operand_r(uint16_t raw, void *Out){
    OperandR *r = (OperandR *)Out;
    //split raw bit, assign to registers
}


void decode_operand_i(uint16_t raw, void *Out){
    OperandI *i = (OperandI *)Out;
    //split the raw operand bit
    //assign to registers
}

void decode_operand_j(uint16_t raw, void *Out){
    OperandJ *j = (OperandJ *)Out;
    
}