#ifndef STYX_ERRORS_H
#define STYX_ERRORS_H

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
  * STYX ERROR HANDLING
  * Comprehensive error handling module
  *============================================================================*/

/*----------------------------------------------------------------------------
  * ERROR CODES
  *----------------------------------------------------------------------------*/
typedef enum {
    /* Success */
    ERR_OK = 0,                             // No Error

    /* ISA Errors */
    ERR_INVALID_OPCODE,                     // Invalid/unknown opcode
    ERR_INVALID_REGISTER,                   // Register number out of range (not 0-15)
    ERR_INVALID_INSN_FORMAT,                // Instruction doesn't match expected format

    /* Memory Errors */
    ERR_INVALID_ADDRESS,                    // Address out of bounds (>= 0x10000)
    ERR_WRITE_PROTECTED,                    // Write to protected region (code during execution)

    /* Execution Errors */
    ERR_DIVISION_BY_ZERO,                   // Division or modulo by zero
    ERR_STACK_OVERFLOW,                     // Stack pointer overflowed into heap
    ERR_STACK_UNDERFLOW,                    // Stack pointer underflowed out of stack region
    ERR_INFINITE_LOOP,                      // Detected infinite loop (HALT condition)

    /* I/O Errors */
    ERR_FILE_NOT_FOUND,                     // Program file not found
    ERR_INVALID_PROGRAM,                    // Invalid program file format
    ERR_FILE_READ,                          // Error reading file
    ERR_FILE_WRITE,                         // Error writing file

    /* General Errors */
    ERR_NULL_POINTER,                       // Null pointer passed to function
    ERR_INVALID_ARGUMENT,                   // Invalid argument to function
    ERR_NOT_IMPLEMENTED,                    // Feature not yet implemented
    ERR_UNKNOWN,                            // Unknown/unspecified error

    ERR_COUNT                               // Total
} ErrorCode_e;

typedef ErrorCode_e error_t;

/*----------------------------------------------------------------------------
  * ERROR CONTEXT
  * Additional context information about an error
  *----------------------------------------------------------------------------*/
typedef struct {
    ErrorCode_e code;
    const char *message;
    const char *file;
    int line;
    uint16_t context_value;
} ErrorContext_t;


#endif /* STYX_ERRORS_H */