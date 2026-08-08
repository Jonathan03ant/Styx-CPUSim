/**
  * @file errors.c
  * @brief ERROR message functons
  */

#include "errors.h"
#include <stdio.h>

/**
 * Error message lookup table
 * Indexed by ErrorCode_e enum values
 */
static const char *error_messages[] = {
    /* Success */
    [ERR_OK] = "Success",

    /* ISA Errors */
    [ERR_INVALID_OPCODE] = "Invalid or unknown opcode",
    [ERR_INVALID_REGISTER] = "Register number out of range (must be 0-15)",
    [ERR_INVALID_INSN_FORMAT] = "Instruction format doesn't match expected format",
    [ERR_INVALID_INSTRUCTION] = "Invalid instruction encoding",

    /* Memory Errors */
    [ERR_INVALID_ADDRESS] = "Memory address out of bounds (must be < 0x10000)",
    [ERR_WRITE_PROTECTED] = "Write to protected memory region (code region during execution)",

    /* Execution Errors */
    [ERR_DIVISION_BY_ZERO] = "Division or modulo by zero",
    [ERR_STACK_OVERFLOW] = "Stack overflow (stack pointer grew into heap)",
    [ERR_STACK_UNDERFLOW] = "Stack underflow (stack pointer below stack region)",
    [ERR_INFINITE_LOOP] = "Infinite loop detected (HALT condition)",

    /* I/O Errors */
    [ERR_FILE_NOT_FOUND] = "Program file not found",
    [ERR_FILE_READ] = "Error reading file",
    [ERR_FILE_WRITE] = "Error writing file",
    [ERR_INVALID_PRG_FORMAT] = "Invalid .prg file format (bad magic number or corrupted header)",
    [ERR_UNSUPPORTED_PRG_VERSION] = "Unsupported .prg format version",
    [ERR_INVALID_ENTRY_POINT] = "Invalid entry point (must be CODE_BASE = 0x0100)",
    [ERR_CODE_TOO_LARGE] = "Code section too large (exceeds CODE region size)",
    [ERR_DATA_TOO_LARGE] = "Data section too large (exceeds DATA region size)",
    [ERR_BSS_TOO_LARGE] = "BSS section too large (exceeds available memory)",

    /* General Errors */
    [ERR_NULL_POINTER] = "Null pointer passed to function",
    [ERR_INVALID_ARGUMENT] = "Invalid argument passed to function",
    [ERR_INVALID_CPU_STATE] = "Invalid CPU state for requested operation",
    [ERR_NOT_IMPLEMENTED] = "Feature not yet implemented",
    [ERR_UNKNOWN] = "Unknown or unspecified error",
};

//* Human readable error message for error code
const char *err_get_message(ErrorCode_e code)
{
    if (code < 0 || code >= ERR_COUNT) {
        return "Invalid error code";
    }

    const char *msg = error_messages[code];
    return msg ? msg : "Unknown error";
}

//* Print error message to stderr
void err_print(ErrorCode_e code)
{
    fprintf(stderr, "Error: %s\n", err_get_message(code));
}

//* Create context with file/line information
ErrorContext_t err_create_context(ErrorCode_e code, const char *file, int line, uint16_t context_value)
{
    ErrorContext_t ctx;
    ctx.code = code;
    ctx.message = err_get_message(code);
    ctx.file = file;
    ctx.line = line;
    ctx.context_value = context_value;
    return ctx;
}

//* Print detailed error context to stderr
void err_print_context(const ErrorContext_t *ctx)
{
    if (ctx == NULL) {
        fprintf(stderr, "Error: NULL error context\n");
        return;
    }

    fprintf(stderr, "Error: %s\n", ctx->message);
    fprintf(stderr, "  Location: %s:%d\n", ctx->file, ctx->line);
    fprintf(stderr, "  Context value: 0x%04X\n", ctx->context_value);
}