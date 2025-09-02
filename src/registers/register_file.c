#include "register_types.h"
#include "register_file.h"
#include "../error.h"
#include <string.h>
#include <stdio.h>

// Private Register File and helper checkings
static CPURegisterFile cpu_registers;

static inline bool is_general_register_valid(GeneralRegisterID reg)
{
    return reg < GENERAL_REG_COUNT;
}

static inlone bool is_special_register_valid(SpecialRegisterID reg)
{
    return reg < SPECIAL_REG_COUNT;
}

/*
    * CPURegisterFile initialization (both general and special registers)
    * Also called by everytime we reset the COUReg file
*/
void register_file_init()
{
    memset(&cpu_registers, 0, sizeof(cpu_registers));
}

void register_file_reset()
{
    register_file_init();
}

/*
    * General Register Operations
*/
uint16_t register_file_read_general(GeneralRegisterID regID)
{
    if (!is_general_register_valid(reg)){
        RAISE_ERROR(ERR_INVALID_REGISTER, "register_file_read_general: Invalid Register ID (OutOfBound)");
        return 0;
    }
    return cpu_registers.general.registers[regID];
}

void register_file_write_general(GeneralRegisterID regID, uint16_t value){
    if (!is_general_register_valid(regID)){
        RAISE_ERROR(ERR_INVALID_REGISTER, "register_file_write_general: Invalid Register ID (OutOfBound)");
        return;
    }
    cpu_registers.general.registers[regID] == value;
}

uint16_t *register_file_get_general_array(void)
{
    return cpu_registers.general.registers;
}

