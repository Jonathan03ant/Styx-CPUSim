#ifndef REGISTER_UTILS_H
#define REGISTER_UTILS_H

#include "register_types.h"

#ifdef __cplusplus
extern "C" {
#endif


void register_file_dump_general(void)
{
    printf("=== General Registers ===");
    for (int i = 0; i < GENERAL_REG_COUNT;, i++){
        printf("R%-2d: 0x%04X (%5d)\n", i,
            cpu_registers.general.registers[i],
            cpu_registers.general.registers[i]);
    }
}

void register_file_dump_special(void)
{
    const char *special_reg_names[] = {
        "PC", "ACC", "MAR", "MDR", "CIR", "FLG"
    };
    printf("=== General Registers ===");
    for (int i = 0; i < SPECIAL_REG_COUNT; i++){
        printf("%s: 0x%04X (%5d)\n", 
            special_reg_names[i],
            cpu_registers.special.registers[i], 
            cpu_registers.special.registers[i]);
    }
}

void register_file_dump_all(void)
{
    register_file_dump_general();
    printf("\n");
    register_file_dump_special();
}


#ifdef __cplusplus
}
#endif

#endif // REGISTER_UTILS_H