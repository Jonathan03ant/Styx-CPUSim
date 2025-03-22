#include "sim_ram.h"
#include "ISA.h"
#include <stdio.h>
#include <stdlib.h>


void init_RAM(RAM *ram)
{
    for (int i = 0; i < RAM_SIZE; i++)
    {
        ram->memory_cell[i] = 0;
    }
}

int read_memory_cell(const RAM *ram, int index)
{
    if (index < 0  || index > RAM_SIZE)
    {
        printf("__ERR_readMem: unknown Memory cell index '%d'\n", index);
        return -1;
    }

    return ram->memory_cell[index];
}

int write_to_memory_cell(RAM *ram, int index, int value)
{
    if (index < 0  || index > RAM_SIZE)
    {
        printf("__ERR_writeToMem: unknown Memory cell index '%d'\n", index);
        return -1;
    }
    
    ram->memory_cell[index] = value;
    return 0;
}


void print_RAM(const RAM *ram)
{
    printf("┌───── SIMULATED RAM ─────┐\n");
    printf("| Addr | Value            |\n");
    printf("|------|------------------|\n");

    for (int i = 0; i < RAM_SIZE; i++) {
        printf("| %4d  | [%04d]           |\n", i, ram->memory_cell[i]);
    }

    printf("└─────────────────────────┘\n");
}