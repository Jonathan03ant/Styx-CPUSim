#include "registers.h"
#include "string.h""

/*
    * This function initializes the registers our cpu require to operate
        * init_Gen_rgstr:  
*/

void init_registers (Registers *regs) 
{
    /*
        initialize Special Registers
    */
    regs->_cpu_sp_rgstr.PC  = 0;
    regs->_cpu_sp_rgstr.ACC = 0;
    regs->_cpu_sp_rgstr.MAR = 0;
    regs->_cpu_sp_rgstr.MDR = 0;
    regs->_cpu_sp_rgstr.CIR = 0;
    regs->_cpu_sp_rgstr.UTG = 0;

    /*
        Inittizlize General Purpose Registers
    */
    for (int i = 0; i < 10; i++)
    {
        regs->_cpu_gn_rgstr.R->value = 0;
        regs->_cpu_gn_rgstr.R->free = true;
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
           regs->_cpu_sp_rgstr.PC,
           regs->_cpu_sp_rgstr.ACC,
           regs->_cpu_sp_rgstr.MAR,
           regs->_cpu_sp_rgstr.MDR,
           regs->_cpu_sp_rgstr.CIR,
           regs->_cpu_sp_rgstr.UTG);
    printf("-------------------------------------------\n");

    /* ─────── General-Purpose Registers ─────── */
    printf("\n[General-Purpose Registers]\n");
    printf("-------------------------------------------\n");
    printf("| Reg  | Value | In Use |\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < 10; i++) {
        printf("| R%-3d | %5d |   %-3s  |\n",
               i,
               regs->_cpu_gn_rgstr.R[i].value,
               regs->_cpu_gn_rgstr.R[i].free ? "NO" : "YES");
    }
    printf("-------------------------------------------\n");

    printf("\n[End of Register Dump]\n\n");
}

int  get_cpu_special_registers(const Registers *regs, const char *reg_name)
{
    if (strcmp(reg_name, "PC")  == 0)
        return regs->_cpu_sp_rgstr.PC;
    if (strcmp(reg_name, "ACC") == 0)
        return regs->_cpu_sp_rgstr.ACC;
    if (strcmp(reg_name, "MAR") == 0)
        return regs->_cpu_sp_rgstr.MAR;
    if (strcmp(reg_name, "MDR") == 0)
        return regs->_cpu_sp_rgstr.MDR;
    if (strcmp(reg_name, "CIR") == 0)
        return regs->_cpu_sp_rgstr.CIR;
    if (strcmp(reg_name, "UTG") == 0)
        return regs->_cpu_sp_rgstr.UTG;

    // Error handling for unknown register name 
    printf("__ERR_RGSTR_sp: unknown register name '%s'\n", reg_name);
    return -1;
}


int get_cpu_general_register(const Registers *regs, int reg_index)
{
    if (reg_index >= 10 || reg_index < 0)
    {
        printf("__ERR_RGSTR_gn: unknown register index '%d'\n", reg_index);
        return -1;
    }

    return regs->_cpu_gn_rgstr.R[reg_index].value;
}


int  set_cpu_special_register(Registers *regs, char *reg_name, int value)
{
    if (strcmp(reg_name, "PC")  == 0)
    {
        regs->_cpu_sp_rgstr.PC = value;
        return 0;
    }
        
    if (strcmp(reg_name, "ACC")  == 0)
    {
        regs->_cpu_sp_rgstr.ACC = value;
        return 0;
    }

    if (strcmp(reg_name, "MAR")  == 0)
    {
        regs->_cpu_sp_rgstr.MAR = value;
        return 0;
    }
        
    if (strcmp(reg_name, "MDR")  == 0)
    {
        regs->_cpu_sp_rgstr.MDR = value;
        return 0;
    }

    if (strcmp(reg_name, "CIR")  == 0)
    {
        regs->_cpu_sp_rgstr.CIR = value;
        return 0;
    }
    
    if (strcmp(reg_name, "UTG")  == 0)
    {
        regs->_cpu_sp_rgstr.UTG = value;
        return 0;
    }
    
    printf("__ERR_RGSTR_sp: Can't set Register!\nunknown register name '%s'\n", reg_name);
    return -1;  

}
