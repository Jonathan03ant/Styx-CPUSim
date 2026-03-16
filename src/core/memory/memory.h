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

#include "../../common/types.h"
#include "../../common/mem_map.h"
#include "../../utils/errors.h"

/**
  * Memory Subsystem struct
  * Opaque, implementation in mem_arry.c
  */
typedef struct Memory_s Memory_t;


Memory_t* mem_create(void);
error_t mem_init(Memory_t *mem);
void mem_destroy(Memory_t *mem);
error_t mem_reset(Memory_t *mem);

/* ===== Byte Access Operations ===== */
error_t mem_read_byte(Memory_t *mem, addr_t addr, byte_t *value);
error_t mem_write_byte(Memory_t *mem, addr_t addr, byte_t value);

/* ===== Word Access Operations ===== */
// Both enforces Little-Endian (low byte, addr +1 or high byte)
error_t mem_read_word(Memory_t *mem, addr_t addr, word_t *value);
error_t mem_write_word(Memory_t *mem, addr_t addr, word_t value);

/* Memory Protection */
error_t mem_set_protection(Memory_t *mem, MemMode_e mode);
error_t mem_get_protection(Memory_t *mem, MemMode_e mode);

/* Program Loader */
error_t mem_load_program(Memory_t *mem, const char *filename);
#endif /* STYX_MEMORY_H */