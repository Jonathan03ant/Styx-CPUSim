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

/*----------------------------------------------------------------------------
  * BASIC HARDWARE TYPES
  *----------------------------------------------------------------------------*/

// Raw instruction bit data type (16 bit instruction word)
typedef uint16_t insn_t;

// Memory address (16 bit address space, 64kb)
typedef uint16_t addr_t;

// Memory bye (8bit) -> mostly used
typedef uint8_t byte_t;

// Memory data word (16 bit data word)
typedef uint16_t word_t;

/*----------------------------------------------------------------------------
  * ISA TYPES
  *----------------------------------------------------------------------------*/

// Opcode (4-bit opcode field)
typedef uint8_t opcode_t;

// General Register address (4 bit, 0-15)
typedef uint8_t regaddr_t;

// General Register value (16bit register content of GRGSTR pointer)
typedef uint16_t regval_t;

/*----------------------------------------------------------------------------
  * INSTRUCTION FIELD TYPE
  *----------------------------------------------------------------------------*/

// 8-bit Immediate value (I-Type A)
typedef uint8_t imm8_t;

// 4-bit offset (I-Type B)
typedef uint8_t offset4_t;

// 12-bit address (J-Type)
typedef uint16_t addr12_t