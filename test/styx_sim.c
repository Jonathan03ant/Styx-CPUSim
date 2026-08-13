/**
 * Styx Simulator - Test Harness
 *
 * Generic test runner for all instruction types
 * Build: cmake . && make
 * Run: ./test/styx_sim isa/test_addi.prg
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// ANSI Color Codes
#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_DIM     "\033[2m"
#define C_GREEN   "\033[92m"
#define C_CYAN    "\033[96m"
#define C_YELLOW  "\033[93m"
#define C_RED     "\033[91m"
#define C_BLUE    "\033[94m"

// Styx CPU Simulator Headers (linked via CMake)
#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "decoder.h"
#include "alu.h"
#include "control.h"
#include "common/types.h"
#include "common/isa_defs.h"
#include "common/reg_defs.h"
#include "common/mem_map.h"
#include "common/prg_format.h"
#include "util/errors.h"

// Snapshot of CPU state for comparison
typedef struct {
    regval_t regs[16];
    addr_t pc;
    flags_t flags;
    addr_t mar;
    word_t mdr;
} CPUSnapshot_t;

void take_snapshot(CPU_t *cpu, CPUSnapshot_t *snap) {
    for (int i = 0; i < 16; i++) {
        reg_read(cpu->registers, i, &snap->regs[i]);
    }
    snap->pc = reg_read_pc(cpu->registers);
    snap->flags = reg_get_flags(cpu->registers);
    snap->mar = cpu->MAR;
    snap->mdr = cpu->MDR;
}

void print_changes(CPUSnapshot_t *before, CPUSnapshot_t *after) {
    bool has_changes = false;

    // Check register changes
    for (int i = 0; i < 16; i++) {
        if (before->regs[i] != after->regs[i]) {
            printf("   %sChanged:%s S%-2d = %s0x%04X%s (%u)\n",
                   C_GREEN, C_RESET, i,
                   C_YELLOW, after->regs[i], C_RESET,
                   after->regs[i]);
            has_changes = true;
        }
    }

    // Check PC change (excluding normal increment)
    if (before->pc != after->pc) {
        printf("   %sChanged:%s PC = %s0x%04X%s\n",
               C_CYAN, C_RESET,
               C_YELLOW, after->pc, C_RESET);
        has_changes = true;
    }

    // Check FLAGS change
    if (before->flags != after->flags) {
        printf("   %sChanged:%s FLAGS = %s0x%04X%s [Z=%d N=%d C=%d V=%d]\n",
               C_BLUE, C_RESET,
               C_YELLOW, after->flags, C_RESET,
               (after->flags & FLAG_Z) ? 1 : 0,
               (after->flags & FLAG_N) ? 1 : 0,
               (after->flags & FLAG_C) ? 1 : 0,
               (after->flags & FLAG_V) ? 1 : 0);
        has_changes = true;
    }

    if (!has_changes) {
        printf("   %s(no changes)%s\n", C_DIM, C_RESET);
    }
}

void print_register_table(CPU_t *cpu, const char *title, regval_t *initial_state) {
    printf("\n%s%s%s\n", C_BOLD, title, C_RESET);
    printf("┌──────┬──────────┬─────────┬────────┐\n");
    printf("│ Reg  │   Hex    │ Decimal │ Label  │\n");
    printf("├──────┼──────────┼─────────┼────────┤\n");

    for (int i = 0; i < 16; i++) {
        regval_t val;
        reg_read(cpu->registers, i, &val);

        // Determine if register changed (for coloring)
        bool changed = (initial_state != NULL) && (val != initial_state[i]);
        bool occupied = (val != 0) || (i == 0) || (i == 13);

        // Labels
        const char *label = "";
        if (i == 0) label = "Zero";
        else if (i == 13) label = "SP";
        else if (i == 14) label = "FP";
        else if (i == 15) label = "RA";

        // Color: green if changed, yellow if occupied, dim if zero
        const char *color = C_RESET;
        if (changed) color = C_GREEN;
        else if (occupied) color = C_YELLOW;
        else color = C_DIM;

        printf("│ %sS%-3d%s │ %s0x%04X%s │ %s%5u%s │ %s%-6s%s │\n",
               color, i, C_RESET,
               color, val, C_RESET,
               color, val, C_RESET,
               color, label, C_RESET);
    }

    printf("└──────┴──────────┴─────────┴────────┘\n");

    // Special registers
    addr_t pc = reg_read_pc(cpu->registers);
    flags_t flags = reg_get_flags(cpu->registers);

    printf("\n%sSpecial Registers:%s\n", C_CYAN, C_RESET);
    printf("   PC    = %s0x%04X%s\n", C_YELLOW, pc, C_RESET);
    printf("   FLAGS = %s0x%04X%s [Z=%d N=%d C=%d V=%d]\n",
           C_YELLOW, flags, C_RESET,
           (flags & FLAG_Z) ? 1 : 0,
           (flags & FLAG_N) ? 1 : 0,
           (flags & FLAG_C) ? 1 : 0,
           (flags & FLAG_V) ? 1 : 0);
}

void print_final_state(CPU_t *cpu, regval_t *initial_regs) {
    printf("\n%s", C_BOLD);
    printf("════════════════════════════════════════════════════════════════\n");
    printf("FINAL STATE\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("%s", C_RESET);

    print_register_table(cpu, "Register File", initial_regs);

    printf("\n%s", C_GREEN);
    printf("════════════════════════════════════════════════════════════════\n");
    printf("Test completed\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("%s\n", C_RESET);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <program.prg>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    // Header
    printf("%s", C_BOLD);
    printf("Styx CPU Simulator - ISA Test Harness\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("%s\n", C_RESET);

    // Create CPU
    CPU_t *cpu = cpu_create();
    if (!cpu) {
        fprintf(stderr, "%s[ERROR]%s Failed to create CPU\n", C_RED, C_RESET);
        return 1;
    }

    // Initialize CPU
    error_t err = cpu_init(cpu);
    if (err != ERR_OK) {
        fprintf(stderr, "%s[ERROR]%s Failed to initialize CPU: %s\n",
                C_RED, C_RESET, err_get_message(err));
        cpu_destroy(cpu);
        return 1;
    }

    // Load program
    printf("%sLoading:%s %s\n", C_CYAN, C_RESET, filename);
    err = mem_load_program(cpu->memory, filename);
    if (err != ERR_OK) {
        fprintf(stderr, "%s[ERROR]%s Failed to load program: %s\n",
                C_RED, C_RESET, err_get_message(err));
        cpu_destroy(cpu);
        return 1;
    }
    printf("   %s[OK]%s Program loaded\n", C_GREEN, C_RESET);
    printf("   %s[OK]%s Entry point: 0x0100\n\n", C_GREEN, C_RESET);

    // Set PC to entry point
    err = reg_write_pc(cpu->registers, 0x0100);
    if (err != ERR_OK) {
        fprintf(stderr, "%s[ERROR]%s Failed to set PC\n", C_RED, C_RESET);
        cpu_destroy(cpu);
        return 1;
    }

    cpu->state = CPU_STATE_LOADED;

    // Capture initial register state
    regval_t initial_regs[16];
    for (int i = 0; i < 16; i++) {
        reg_read(cpu->registers, i, &initial_regs[i]);
    }

    // Show initial register state
    printf("%s", C_BOLD);
    printf("════════════════════════════════════════════════════════════════\n");
    printf("INITIAL STATE\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("%s", C_RESET);

    print_register_table(cpu, "Register File (Before Execution)", NULL);

    // Start timing
    clock_t start_time = clock();

    // Execution header
    printf("%s", C_BOLD);
    printf("════════════════════════════════════════════════════════════════\n");
    printf("EXECUTION\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("%s\n", C_RESET);

    // Run program with change tracking
    int max_steps = 100;
    CPUSnapshot_t before, after;

    for (int step = 0; step < max_steps; step++) {
        // Take snapshot before execution
        take_snapshot(cpu, &before);
        addr_t pc = before.pc;

        // Check if halted or error
        if (cpu->state == CPU_STATE_HALTED) {
            // End timing here
            clock_t halt_time = clock();
            double halt_ms = ((double)(halt_time - start_time)) / CLOCKS_PER_SEC * 1000.0;

            printf("\n%s[HALT]%s CPU halted at cycle %d (%.3f ms)\n\n",
                   C_RED, C_RESET, step, halt_ms);
            break;
        }
        if (cpu->state == CPU_STATE_ERROR) {
            printf("%s[ERROR]%s %s\n", C_RED, C_RESET, err_get_message(cpu->last_error));
            break;
        }

        // Fetch instruction for display
        word_t instr = 0;
        mem_read_word(cpu->memory, pc, &instr);

        // Execute one step
        err = cpu_step(cpu);
        if (err != ERR_OK) {
            fprintf(stderr, "%s[ERROR]%s Step failed: %s\n",
                    C_RED, C_RESET, err_get_message(err));
            break;
        }

        // Take snapshot after execution
        take_snapshot(cpu, &after);

        // Print cycle info
        printf("%s[Cycle %d]%s %s0x%04X%s: %s0x%04X%s\n",
               C_CYAN, step, C_RESET,
               C_DIM, pc, C_RESET,
               C_YELLOW, instr, C_RESET);

        // Print changes
        print_changes(&before, &after);
        printf("\n");
    }

    if (cpu->state != CPU_STATE_HALTED) {
        printf("%s[WARNING]%s CPU did not halt after %d steps\n",
               C_YELLOW, C_RESET, max_steps);
    }

    // Print final state
    print_final_state(cpu, initial_regs);

    // Cleanup
    cpu_destroy(cpu);

    return 0;
}
