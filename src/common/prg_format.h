#ifndef STYX_PRG_FORMAT_H
#define STYX_PRG_FORMAT_H

#include "types.h"
#include <stdint.h>

/**
 * STYX .PRG FILE FORMAT DEFINITION
 *
 * defines the binary executable format for styx programs
 * Shared between loader (mem_loader.c) and assembler (styx-asm)
 *
 * File structure
 *      [Header: 16 Bytes]
 *      [Code section: code_size bytes]
 *      [Data section: data_size bytes]
 *      (BSS section, NOT in file, allocatred at load time)
 */



#endif /* STYX_PRG_FORMAT_H */