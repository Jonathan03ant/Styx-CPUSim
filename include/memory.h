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

#include "common/types.h"
#include "common/mem_map.h"
#include "util/errors.h"

/**
  * * Memory Subsystem struct
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
error_t mem_get_protection(Memory_t *mem, MemMode_e *mode);

/* Program Loader */
error_t mem_load_program(Memory_t *mem, const char *filename);
error_t mem_load_from_bytes(
    Memory_t* mem,
    const uint8_t* code_bytes,
    size_t code_size,
    const uint8_t* data_bytes,
    size_t data_size
);

/* Memory Usage Statistics */
void mem_set_usage(Memory_t* mem, uint16_t code, uint16_t data, uint16_t bss);
void mem_get_usage(const Memory_t* mem, uint16_t* code, uint16_t* data, uint16_t* bss);

#endif /* STYX_MEMORY_H */