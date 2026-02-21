#ifndef STYX_MEM_MAP_H
#define STYX_MEM_MAP_H

#include <stdint.h>
#include <stdbool.h>
#include "types.h"

/*============================================================================
  * STYX MEMORY MAP DEFINITIONS
  *
  * Memory layout for 64KB address space (0x0000 to 0xFFFF)
  * Defines region boundaries, sizes, and address validation
  *============================================================================*/

/*----------------------------------------------------------------------------
  * MEMORY REGION BOUNDARIES
  * 64KB address space divided in to 5 groups
  * - Reserved: Entry point and interrupt vectors
  * - Code: program instruction (read only during execution)
  * - Data: Global and static variables
  * - Heap: Dynamic Memory allocation, grows upward
  * - Stack: Function calls and locals, grows downwards
  *----------------------------------------------------------------------------*/

// Reserved Region: 0x0000 - 0x00FF (256 bytes)
#define MEM_RESERVED_BASE     0x0000
#define MEM_RESERVED_END      0x00FF

// Code Region: 0x0100 - 0x1FFF (~8 KB)
#define MEM_CODE_BASE         0x0100
#define MEM_CODE_END          0x1FFF

// Data Region: 0x2000 - 0x7FFF (~24 KB)
#define MEM_DATA_BASE        0x2000
#define MEM_DATA_END         0x7FFF

// Heap Region: 0x8000 - 0xDFFF (~24 KB)
#define MEM_HEAP_BASE        0x8000
#define MEM_HEAP_END         0xDFFF

// Stack Region: 0xE000 - 0xFFFF (~8 KB)
#define MEM_STACK_BASE       0xE000
#define MEM_STACK_END        0xFFFF

#endif /* STYX_MEM_MAP */
