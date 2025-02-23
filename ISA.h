#ifndef ISA_H
#define ISA_H

/*
    * This enum defines the Instruction Set Architecture (ISA) for the CPU.
    * The ISA only defines the opcodes.
*/

typedef enum {
    /* I/O Instructions */
    INP  = 19,     // Input number into ACC                        Class: I/O
    OUT  = 99,    // Output ACC value                             Class: I/O    

    /* Memory Instructions */
    LDD = 10,     // Load value from memory into ACC              Class: Mem
    STR = 20,     // Store ACC value into memory                  Class: Mem
    PSH = 30,     // Push Value on to the stack                   Class: Mem
    POP = 40,     // Pop Value from the stack                     Class: Mem

    /* Arithmetic Instructions */
    ADD  = 12,    // Add memory value into ACC                    Class: Arth
    SUB  = 22,    // Subtract memory value from ACC               Class: Arth
    MUL  = 32,    // Multiply ACC by memory value                 Class: Arth
    DIV  = 42,    // Divide ACC by memory value                   Class: Arth
    CMP  = 52,    // Compare ACC with memory                      Class: Arth

    /* Control Flow Instructions */
    JMP  = 5,     // Unconditional Jump                           Class: Ctl
    BRZ  = 15,    // Jump only if ACC is zero                     Class: Ctl
    BRP  = 25,    // Branch if ACC > 0                            Class: Ctl
    BRN  = 35,    // Branch if ACC < 0                            Class: Ctl

    /* Execution Control */
    NOP  = 59,    // No operation
    INT  = 69,    // Interrupt
    HLT  = 79,    // Halt execution                               Class: Exe
    ERR  = 89,    // Error                                        Class: Exe
} ISA;

#endif // ISA_H

