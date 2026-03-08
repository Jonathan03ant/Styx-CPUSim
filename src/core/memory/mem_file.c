/**
  * @file mem_file.c
  * @brief Memory array implementation - 64KB byte-addressable memory
  */

#include "memory.h"
#include "../../common/mem_map.h"
#include "../../utils/bitops.h"
#include <stdlib.h>
#include <string.h>



/**
 * Memory Subsystem structure
 * 64kb of array and protection mode
 */
struct Memory_s {
    byte_t data[MEM_TOTAL_SIZE];                // 64KB memory array (0x0000 - 0xFFFF)
    MemMode_e mode;                             // Protection mode (LOADING or RUNNING)
};