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
#define MEM_RESERVED_BASE       0x0000
#define MEM_RESERVED_END        0x00FF

// Code Region: 0x0100 - 0x1FFF (~8 KB)
#define MEM_CODE_BASE           0x0100
#define MEM_CODE_END            0x1FFF

// Data Region: 0x2000 - 0x7FFF (~24 KB)
#define MEM_DATA_BASE           0x2000
#define MEM_DATA_END            0x7FFF

// Heap Region: 0x8000 - 0xDFFF (~24 KB)
#define MEM_HEAP_BASE           0x8000
#define MEM_HEAP_END            0xDFFF

// Stack Region: 0xE000 - 0xFFFF (~8 KB)
#define MEM_STACK_BASE          0xE000
#define MEM_STACK_END           0xFFFF

/*----------------------------------------------------------------------------
  * MEMORY REGION SIZES
  * Calculated from boundaries (END - BASE + 1)
  * All regions use inclusive addressing
  *----------------------------------------------------------------------------*/
#define MEM_TOTAL_SIZE          0x10000 // 65536 bytes (64 KB)
#define MEM_RESERVED_SIZE       (MEM_RESERVED_END - MEM_RESERVED_BASE + 1)  // 256 bytes
#define MEM_CODE_SIZE           (MEM_CODE_END - MEM_CODE_BASE + 1)          // 7936 bytes (~8 KB)
#define MEM_DATA_SIZE           (MEM_DATA_END - MEM_HEAP_BASE + 1)          // 24576 bytes (~24 KB)
#define MEM_HEAP_SIZE           (MEM_HEAP_END - MEM_HEAP_BASE + 1)          // 24576 bytes (~24 KB)
#define MEM_STACK_SIZE          (MEM_STACK_END - MEM_STACK_BASE + 1)        // 8192 bytes (~8 KB)

/*----------------------------------------------------------------------------
  * SPECIAL ADDRESSES
  *----------------------------------------------------------------------------*/
#define MEM_ENTRY_POINT      0x0000     // PC is initialized to this address
#define MEM_STACK_TOP        0xFFFF     // SP is initialized to this address

/*----------------------------------------------------------------------------
  * ADDRESS VALIDATION FUNCTIONS
  *----------------------------------------------------------------------------*/

static inline bool is_reserved_region(addr_t addr)
{
    return addr >= MEM_RESERVED_BASE && addr <= MEM_RESERVED_END;
}

static inline bool is_code_region(addr_t addr)
{
    return addr >= MEM_CODE_BASE && addr <= MEM_CODE_END;
}

static inline bool is_data_region(addr_t addr)
{
    return addr >= MEM_DATA_BASE && addr <= MEM_DATA_END;
}

static inline bool is_heap_region(addr12_t addr)
{
    return addr >= MEM_HEAP_BASE && addr <= MEM_HEAP_END;
}

static inline bool is_stack_region(addr_t addr)
{
    return addr >= MEM_STACK_BASE && addr <= MEM_STACK_END;
}

static inline bool is_valid_region(addr_t addr)
{
     return addr <= MEM_STACK_END;
}
#endif /* STYX_MEM_MAP */
