/**
 * @file styx_api.c
 * @brief Styx Simulator - High-Level API Implementation
 *
 * Facade implementation providing simple wrapper functions for external tools.
 * Wraps complex CPU/memory/register operations into clean, easy-to-use API.
 *
 */

#include "styx_api.h"
#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "decoder.h"
#include "common/types.h"
#include "common/isa_defs.h"
#include "common/reg_defs.h"
#include "util/errors.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct Simulator_s {
    CPU_t* cpu;              // The CPU instance
};

//* Disassembly format strings (indexed by opcode)
static const char* disasm_formats[16] = {
    "NOP",                  // 0x0
    "MOV S%d, S%d",         // 0x1
    "LOAD S%d, S%d, %d",    // 0x2
    "STORE S%d, S%d, %d",   // 0x3
    "ADD S%d, S%d, S%d",    // 0x4
    "SUB S%d, S%d, S%d",    // 0x5
    "MUL S%d, S%d, S%d",    // 0x6
    "DIV S%d, S%d, S%d",    // 0x7
    "ADDI S%d, 0x%02X",     // 0x8
    "CMP S%d, S%d",         // 0x9
    "JMP 0x%04X",           // 0xA
    "JZ 0x%04X",            // 0xB
    "JNZ 0x%04X",           // 0xC
    "CALL 0x%04X",          // 0xD
    "RET",                  // 0xE
    "LUI S%d, 0x%02X"       // 0xF
};

//* Create a new Styx simulator instance
Simulator_t* styx_create(void)
{
    /*
        * Allocate simulator struct
        * Create CPU instance
        * Return simulator or NULL on failure
    */

    Simulator_t *sim = (Simulator_t*)malloc(sizeof(Simulator_t));
    if (sim == NULL) {
        return NULL;
    }

    // Create CPU (registers + memory + all sub systems)
    sim->cpu = cpu_create();
    if (sim->cpu == NULL) {
        free(sim);
        return NULL;
    }

    // Initialize CPU to IDLE state
    cpu_init(sim->cpu);

    return sim;
}

//* Destroy simulator and free all resources
void styx_destroy(Simulator_t* sim)
{
    /*
       * Check if simulator is valid
       * Destroy CPU (frees registers + memory)
       * Free simulator struct
    */

    if (sim == NULL) {
        return;
    }

    // Destroy CPU and all subsystems
    if (sim->cpu != NULL) {
        cpu_destroy(sim->cpu);
    }

    free(sim);
}

//* Load a .prg binary file into simulator memory
error_t styx_load_program(Simulator_t* sim, const char* filepath)
{
    /*
       * Validate inputs
       * Call memory loader
       * Return error code
    */

    if (sim == NULL || filepath == NULL) {
        return ERR_NULL_POINTER;
    }

    if (sim->cpu == NULL || sim->cpu->memory == NULL) {
        return ERR_NULL_POINTER;
    }

    // Load program into memory (validates header, loads code/data sections)
    error_t err = mem_load_program(sim->cpu->memory, filepath);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

//* Reset simulator to initial state
error_t styx_reset(Simulator_t* sim)
{
    /*
       * Validate simulator
       * Call cpu_reset (resets PC, registers, FLAGS)
       * Memory remains loaded
    */

    if (sim == NULL || sim->cpu == NULL) {
        return ERR_NULL_POINTER;
    }

    // Reset CPU state (PC=0x0100, registers cleared, FLAGS=0)
    cpu_reset(sim->cpu);

    return ERR_OK;
}

//* Execute one instruction (single step)
error_t styx_step(Simulator_t* sim)
{
    /*
       * Validate simulator
       * Call cpu_step (fetch-decode-execute one instruction)
       * Return error code
    */

    if (sim == NULL || sim->cpu == NULL) {
        return ERR_NULL_POINTER;
    }

    // Execute one instruction
    error_t err = cpu_step(sim->cpu);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

//* Run until HALT or error
error_t styx_run(Simulator_t* sim)
{
    /*
       * Validate simulator
       * Call cpu_run (executes until HALT)
       * Return error code
    */

    if (sim == NULL || sim->cpu == NULL) {
        return ERR_NULL_POINTER;
    }

    // Run until HALT or error
    error_t err = cpu_run(sim->cpu);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

//* Get snapshot of current CPU state
CPUSnapshot_t styx_get_snapshot(const Simulator_t* sim)
{
    /*
       * Create empty snapshot
       * If simulator valid, copy all CPU state
       * Read all registers, PC, FLAGS
       * Copy internal registers (MAR, MDR, IR)
       * Copy execution state (state, cycle_count, last_error)
    */

    CPUSnapshot_t snap;
    memset(&snap, 0, sizeof(CPUSnapshot_t));

    // If simulator invalid, return zeroed snapshot
    if (sim == NULL || sim->cpu == NULL) {
        snap.state = CPU_STATE_ERROR;
        snap.last_error = ERR_NULL_POINTER;
        return snap;
    }

    CPU_t* cpu = sim->cpu;

    // Copy all 16 registers
    for (int i = 0; i < 16; i++) {
        reg_read(cpu->registers, i, &snap.regs[i]);
    }

    // Copy special registers
    snap.pc = reg_read_pc(cpu->registers);
    snap.flags = reg_get_flags(cpu->registers);

    // Copy internal registers
    snap.mar = cpu->MAR;
    snap.mdr = cpu->MDR;
    snap.ir = cpu->IR;

    // Copy execution state
    snap.state = (int)cpu->state;
    snap.cycle_count = cpu->cycle_count;
    snap.last_error = (int)cpu->last_error;

    return snap;
}

//* Read a word from memory at specified address
error_t styx_read_memory(const Simulator_t* sim, addr_t address, word_t* value)
{
    /*
       * Validate inputs
       * Call mem_read_word
       * Return error code
    */

    if (sim == NULL || value == NULL) {
        return ERR_NULL_POINTER;
    }

    if (sim->cpu == NULL || sim->cpu->memory == NULL) {
        return ERR_NULL_POINTER;
    }

    // Read word from memory
    error_t err = mem_read_word(sim->cpu->memory, address, value);
    if (err != ERR_OK) {
        return err;
    }

    return ERR_OK;
}

//* Get memory usage statistics for all regions
error_t styx_get_memory_stats(const Simulator_t* sim, MemoryStats_t* stats)
{
    /*
       * Validate inputs
       * Get usage from memory subsystem
       * BSS and data combined into data_used
    */

    if (sim == NULL || stats == NULL) {
        return ERR_NULL_POINTER;
    }

    if (sim->cpu == NULL || sim->cpu->memory == NULL) {
        return ERR_NULL_POINTER;
    }

    // Get usage from memory
    uint16_t code, data, bss;
    mem_get_usage(sim->cpu->memory, &code, &data, &bss);

    stats->code_used = code;
    stats->data_used = data + bss;  // Data + BSS combined
    stats->heap_used = 0;           // Not tracked (dynamic allocation not implemented)
    stats->stack_used = 0;          // Not tracked (would need SP monitoring)

    return ERR_OK;
}

//* Disassemble instruction at address into human-readable text
error_t styx_disassemble(const Simulator_t* sim, addr_t address, char* buffer, size_t buf_size)
{
    /*
    * Validate inputs
    * Read instruction from memory
    * Decode instruction
    * Format based on instruction format type
    */

    if (sim == NULL || buffer == NULL || buf_size == 0) {
        return ERR_NULL_POINTER;
    }

    if (sim->cpu == NULL || sim->cpu->memory == NULL) {
        return ERR_NULL_POINTER;
    }

    // Read instruction from memory
    word_t raw_insn;
    error_t err = mem_read_word(sim->cpu->memory, address, &raw_insn);
    if (err != ERR_OK) {
        snprintf(buffer, buf_size, "<read error>");
        return err;
    }

    // Decode instruction
    DecodedInstruction_t decoded;
    err = decode_instruction(raw_insn, &decoded);
    if (err != ERR_OK) {
        snprintf(buffer, buf_size, "<invalid>");
        return err;
    }

    // Get format string from LUT
    const char* fmt = disasm_formats[decoded.opcode];

    // Extract fields
    reg_t rd = decoded_get_rd(&decoded);
    reg_t rs1 = decoded_get_rs1(&decoded);
    reg_t rs2 = decoded_get_rs2(&decoded);
    uint16_t imm = decoded_get_immediate(&decoded);

    // Format based on instruction format
    switch (decoded.format) {
        case INSN_FORMAT_R:
            snprintf(buffer, buf_size, fmt, rd, rs1, rs2);
            break;
        case INSN_FORMAT_I_A:
            snprintf(buffer, buf_size, fmt, rd, imm);
            break;
        case INSN_FORMAT_I_B:
            snprintf(buffer, buf_size, fmt, rd, rs1, imm);
            break;
        case INSN_FORMAT_J:
            snprintf(buffer, buf_size, fmt, imm);
            break;
        default:
            snprintf(buffer, buf_size, "<unknown format>");
            return ERR_INVALID_INSTRUCTION;
    }

    return ERR_OK;
}