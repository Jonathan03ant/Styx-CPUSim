#ifndef STYX_TYPES_H
#define STYX_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
  * STYX COMMON TYPES
  *
  * Foundational type definitions used across all modules
  * This is the single source of truth for all Styx data types
  *============================================================================*/

/**
 ** BASIC HW TYPES
 */

// Raw instruction bit data type (16 bit instruction word)
typedef uint16_t insn_t;

// Memory address (16 bit address space, 64kb)
typedef uint16_t addr_t;

// Memory bye (8bit) -> mostly used
typedef uint8_t byte_t;

// Memory data word (16 bit data word)
typedef uint16_t word_t;

/**
 ** ISA TYPES
 */

// Opcode (4-bit opcode field)
typedef uint8_t opcode_t;

// General Register address (4 bit, 0-15)
typedef uint8_t reg_t;

// General Register value (16bit register content of GRGSTR pointer)
typedef uint16_t regval_t;

/**
 ** ISA FIELD TYPES
 */

// 8-bit Immediate value (I-Type A)
typedef uint8_t imm8_t;

// 4-bit offset (I-Type B)
typedef uint8_t offset4_t;

// 12-bit address (J-Type)
typedef uint16_t addr12_t;

/**
 **FLAG TYPES
 */

// FLAGS register (4 bits used: Z, N, C, V)
typedef uint16_t flags_t;

// Individual flag bits
typedef bool flag_bit_t;

/**
 ** CPU STATE SNAPSHOT
  * Frozen copy of CPU state at a specific moment
  * Used by TUI and external tools to inspect CPU state
*/
typedef struct {
    // ISA-visible state
    regval_t regs[16];        // General-purpose registers S0-S15
    addr_t pc;                // Program Counter
    flags_t flags;            // FLAGS register (Z, N, C, V)

    // Internal registers (for debugging/education)
    addr_t mar;               // Memory Address Register
    word_t mdr;               // Memory Data Register
    insn_t ir;                // Instruction Register

    // Execution state
    int state;                // CPU state (cast to CPUState_e when using)
    uint32_t cycle_count;     // Total instructions executed
    int last_error;           // Last error code (cast to error_t when using)
} CPUSnapshot_t;

#endif /* STYX_TYPES_H */