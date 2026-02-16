#ifndef STYX_REG_DEFS_H
#define STYX_REG_DEFS_H

#include "types.h"

/*============================================================================
  * STYX REGISTER DEFINITIONS
  *
  * Register numbers, conventions, and FLAGS bit definitions
  * ISA-visible registers only (S0-S15, PC, FLAGS)
  *============================================================================*/

/*----------------------------------------------------------------------------
  * REGISTER CONVENTION ALIASES
  *----------------------------------------------------------------------------*/
#define REG_SP REG_S13          // Stack Pointer (Used by CALL/RET)
#define REG_FP REG_S14          // Frame Pointer
#define REG_HP REG_S14          // HEAP Pointer, Alternate use of S14
#define REG_RA REG_S15          // Return Address (Used by CALL/RET)

/*----------------------------------------------------------------------------
  * FLAG REGISTR BIT DEFINITION
  * FLAGS is a 16 bit register, but only 4 bits are used (bit 0-3)
  * CMP instruction sets FLAGS, JZ/JNZ reads the Z flag
  *----------------------------------------------------------------------------*/
#define FLAG_Z (1 << 0)         // Zero flag, bit 0
#define FLAG_N (1 << 1)         // Negative flag, bit 1
#define FLAG_C (1 << 2)         // Carry flag, bit 2
#define FLAG_V (1 << 3)         // Overflow flag, bit 3

/*----------------------------------------------------------------------------
  * REGISTER INITIALIZATION
  * Initial register values on cpu reset/power on
  *----------------------------------------------------------------------------*/
#define REG_INIT_ZERO   0x0000  // S0-S12, S14, S15 initialized to zero
#define REG_INIT_SP     0xFFFF  // S13(SP) initialized to top of the stack
#define REG_INIT_PC     0x0000  // PC initialized to entery point
#define REG_INIT_FLAGS  0x0000  // FLAGS initialized to all clear

/*----------------------------------------------------------------------------
  * GENERAL PURPOSE REGISTERS
  * 16 registers (S0-S15), each 16 bits wide
  *----------------------------------------------------------------------------*/
typedef enum {
    REG_S0  = 0x0,    // Hardwired to zero
    REG_S1  = 0x1,    // General purpose
    REG_S2  = 0x2,    // General purpose
    REG_S3  = 0x3,    // General purpose
    REG_S4  = 0x4,    // General purpose
    REG_S5  = 0x5,    // General purpose
    REG_S6  = 0x6,    // General purpose
    REG_S7  = 0x7,    // General purpose
    REG_S8  = 0x8,    // General purpose
    REG_S9  = 0x9,    // General purpose
    REG_S10 = 0xA,    // General purpose
    REG_S11 = 0xB,    // General purpose
    REG_S12 = 0xC,    // General purpose
    REG_S13 = 0xD,    // Stack Pointer (convention)
    REG_S14 = 0xE,    // Frame/Heap Pointer (convention)
    REG_S15 = 0xF,    // Return Address (convention)

    REG_COUNT = 16    // Total number of GPRs
} RegNum_e;


#endif /* STYX_REG_DEFS_H */