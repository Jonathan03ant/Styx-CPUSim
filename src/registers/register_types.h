/*
    Contains all register types
*/
#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include "stdio.h"
#include "stdlib.h"
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif


/*
    * General Purpose registers (R0-R15)
    * 16 Registers, each 16 bit addresable 
*/
typedef enum {
    GENERAL_REG_R0,
    GENERAL_REG_R1,
    GENERAL_REG_R2,
    GENERAL_REG_R3,
    GENERAL_REG_R4,
    GENERAL_REG_R5,
    GENERAL_REG_R6,
    GENERAL_REG_R7,
    GENERAL_REG_R8,
    GENERAL_REG_R9,
    GENERAL_REG_R10,
    GENERAL_REG_R11,
    GENERAL_REG_R12,
    GENERAL_REG_R13,
    GENERAL_REG_R14,
    GENERAL_REG_R15,
    GENERAL_REG_COUNT
}GeneralRegisterID;

/*
    * Special Purpose Registers
*/

typedef enum {
    SPECIAL_REG_PC,           // Program Counter: Address of next instruction
    SPECIAL_REG_ACC,          // Accumulator: Arithmetic and logic results
    SPECIAL_REG_MAR,          // Memory Address Register: Memory address to access
    SPECIAL_REG_MDR,          // Memory Data Register: Data read/written to memory
    SPECIAL_REG_CIR,          // Current Instruction Register: Current instruction
    SPECIAL_REG_FLAGS,        // Status Flags Register
    SPECIAL_REG_COUNT
}SpecialRegisterID;

/*
    * Register File structures
*/
typedef struct {
    uint16_t registers[GENERAL_REG_COUNT];
}GeneralRegisterFile;

typedef struct {
    uint16_t registers[SPECIAL_REG_COUNT];
}SpecialRegisterFile;

typedef struct {
    GeneralRegisterFile general;
    SpecialRegisterFile special;
}CPURegisterFile;

extern CPURegisterFile cpu_registers;

#ifdef __cplusplus
}
#endif

#endif 