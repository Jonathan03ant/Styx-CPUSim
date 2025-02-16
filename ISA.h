#ifndef ISA_H
#define ISA_H

/*
    * This enum defines the Instruction Set Architecture (ISA) for the CPU.
    * The ISA only defines the opcodes.
*/

typedef enum {
    /* I/O Instructions */
    INP  = 90,   // Input number into ACC                      Class: I/O
    OUT  = 07,   // Output ACC value                           Class: I/O    

    /* Memory Instructions */
    LOAD = 01,   // Load value from memory into ACC            Class: Mem
    STORE = 02,  // Store ACC value into memory                Class: Mem

    /* Arithmetic Instructions */
    ADD  = 03,   // Add memory value into ACC                  Class: Arth
    SUB  = 04,   // Subtract memory value from ACC             Class: Arth
    MUL  = 12,   // Multiply ACC by memory value               Class: Arth
    DIV  = 13,   // Divide ACC by memory value                 Class: Arth
    CMP  = 09,   // Compare ACC with memory                    Class: Arth

    /* Control Flow Instructions */
    JMP  = 05,   // Unconditional Jump                         Class: Ctl
    BRZ  = 06,   // Jump only if ACC is zero                   Class: Ctl
    BRP  = 10,   // Branch if ACC > 0                          Class: Ctl
    BRN  = 11,   // Branch if ACC < 0                          Class: Ctl

    /* Execution Control */
    HLT  = 08    // Halt execution                             Class: Exe
} ISA;

#endif // ISA_H

