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