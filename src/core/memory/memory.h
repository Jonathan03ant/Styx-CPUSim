/**
  * @file memory.h
  * @brief Memory subsystem public API
  *
  * 64KB byte-addressable memory with little-endian byte ordering.
  */

#ifndef STYX_MEMORY_H
#define STYX_MEMORY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../common/types.h"
#include "../../utils/errors.h"

/**
  * Memory Subsystem struct
  * Opaque, implementation in mem_arry.c
  */
typedef struct Memory_s Memory_t;

/**
 * Protection Modes
 */
typedef enum {
    MEM_MODE_LOADING,
    MEM_MODE_RUNNING
} MemoryMode_e;







#endif /* STYX_MEMORY_H */