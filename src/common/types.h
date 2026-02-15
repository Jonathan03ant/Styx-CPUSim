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