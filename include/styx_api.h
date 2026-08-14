/**
 * @file styx_api.h
 * @brief Styx Simulator - High-Level Public API
 *
 * Facade API for external tools (TUI, debuggers, test frameworks).
 * Provides simple, high-level functions to control the simulator
 * without directly accessing internal CPU/memory/register structures.
 *
 * This API is designed for:
 *   - Python TUI (via ctypes)
 *   - External debuggers
 *   - Automated testing tools
 */
#ifndef STYX_API_H
#define STYX_API_H

#include <stddef.h>
#include "common/types.h"
#include "../util/errors.h"

typedef struct Simulator_s Simulator_t;


Simulator_t* styx_create(void);                                                                     //* Create a new Styx Simulator instance
void styx_destroy(Simulator_t* sim);                                                                //* Destroy simulator and free all resource
error_t styx_load_program(Simulator_t* sim, const char* filepath);                                  //* Load a .prg binary file into simulator memory
error_t styx_reset(Simulator_t* sim);                                                               //* Reset simulator to initial state
error_t styx_step(Simulator_t* sim);                                                                //* Execute one instruction (single step)
error_t styx_run(Simulator_t* sim);                                                                 //* Run until HALT or error
CPUSnapshot_t styx_get_snapshot(const Simulator_t* sim);                                            //* Get snapshot of current CPU state
error_t styx_read_memory(const Simulator_t* sim, addr_t address, word_t* value);                    //* Read a word from memory at specified address
error_t styx_get_memory_stats(const Simulator_t* sim, MemoryStats_t* stats);                        //* Get memory usage statistics for all regions
error_t styx_disassemble(const Simulator_t* sim, addr_t address, char* buffer, size_t buf_size);    //* Disassemble instruction at address into human-readable text
error_t styx_load_from_bytes(
    Simulator_t* sim,
    const uint8_t* code_bytes,
    size_t code_size,
    const uint8_t* data_bytes,
    size_t data_size
);                                                                                                  //* Load program from memory buffers (no file I/O)
#endif /* STYX_API_H */