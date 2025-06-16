#include "isa.h"
#include "operand.h"


void decode_operand_r(uint16_t raw, void *Out){
    OperandR *r = (OperandR *)Out;
    /*
        [opcode 4 bits] [raw 12 bits]
        raw = 0000 0000 0000
        
        Rd = bit 8-11
        Rs1 = bit 4-7
        Rs2 = bit 0-3

        raw = 0xAB3 = 10:11:3 and 1010 1011 0011
        1011 1011 0011
        rd = shift, extract
        rd = (raw >> 8 ) & 0xF
        raw >> 8 = 1010 1011 0011 >> 8 = 0000 0000 1010
        then 0000 0000 1010 & 0xF (Extract)
        0000 0000 1010
                  1111
             rd = 1011

        rs1 = shift, extract (middle)
        rd = (raw >> 4) & 0xF
        raw >> 4 = 1010 1011 0011 >> 4 =  0000 1010 1011
        then 0000 1010 1011  & 0xF
        0000 1010 1011
             0000 1111
            rs1 = 1011
    */
    r->rd = (raw >> 8) & 0xF;
    r->rs1 = (raw >> 4) & 0xF;
    r->rs2 = (raw) & 0xF;
    
}


void decode_operand_i(uint16_t raw, void *Out){
    OperandI *i = (OperandI *)Out;

    i->rd = (raw >> 8) & 0xF;
    i->rs1 = (raw >> 4) & 0xF;
    i->imm = (raw) & 0xF;

}

void decode_operand_j(uint16_t raw, void *Out){
    OperandJ *j = (OperandJ *)Out;
    j->imm = raw;

}