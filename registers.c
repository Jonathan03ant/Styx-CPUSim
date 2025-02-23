#include "registers.h"

/*
    * This function initializes the registers our cpu require to operate
        * init_Gen_rgstr:  
*/

void init_registers (Registers *regs) 
{
    /*
        initialize Special Registers
    */
    regs->sp_rgstr.PC  = 0;
    regs->sp_rgstr.ACC = 0;
    regs->sp_rgstr.MAR = 0;
    regs->sp_rgstr.MDR = 0;
    regs->sp_rgstr.CIR = 0;
    regs->sp_rgstr.UTG = 0;

    /*
        Inittizlize General Purpose Registers
    */
    for (int i = 0; i < 10; i++)
    {
        regs->gn_rgstr.R->value = 0;
        regs->gn_rgstr.R->in_use = 0;
    }
}

/*
    * Cool print function
*/
void print_registers(const Registers *regs) {
    printf("┌─────────────────────────────────────────┐\n");
    printf("│        CPU REGISTER STATE DUMP          │\n");
    printf("└─────────────────────────────────────────┘\n");

    /* ─────── Special-Purpose Registers ─────── */
    printf("\n[Special-Purpose Registers]\n");
    printf("-------------------------------------------\n");
    printf("| %-4s | %-4s | %-4s | %-4s | %-4s | %-4s |\n",
           "PC", "ACC", "MAR", "MDR", "CIR", "UTG");
    printf("| %4d | %4d | %4d | %4d | %4d | %4d |\n",
           regs->sp_rgstr.PC,
           regs->sp_rgstr.ACC,
           regs->sp_rgstr.MAR,
           regs->sp_rgstr.MDR,
           regs->sp_rgstr.CIR,
           regs->sp_rgstr.UTG);
    printf("-------------------------------------------\n");

    /* ─────── General-Purpose Registers ─────── */
    printf("\n[General-Purpose Registers]\n");
    printf("-------------------------------------------\n");
    printf("| Reg  | Value | In Use |\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < 10; i++) {
        printf("| R%-3d | %5d |   %-3s  |\n",
               i,
               regs->gn_rgstr.R[i].value,
               regs->gn_rgstr.R[i].in_use ? "YES" : "NO");
    }
    printf("-------------------------------------------\n");

    printf("\n[End of Register Dump]\n\n");
}