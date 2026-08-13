#ifndef STYX_PRG_FORMAT_H
#define STYX_PRG_FORMAT_H

#include "types.h"
#include <stdint.h>

#define PRG_MAGIC_0 'S'                             //0x53
#define PRG_MAGIC_1 'T'                             //0x54
#define PRG_MAGIC_2 'Y'                             //0x59
#define PRG_MAGIC_3 'X'                             //0x58

#define PRG_VERSION_1  1
#define PRG_VERSION    PRG_VERSION_1
#define PRG_FLAGS_NONE 0x00

/*
 * FILE LAYOUT
 *
 * .prg file structure:
 *   Offset 0x00-0x0F:  Header (16 bytes)
 *   Offset 0x10+:      Code section (code_size bytes)
 *   Offset code+:      Data section (data_size bytes)
 *
 * Memory layout after loading:
 *   CODE_BASE (0x0100): Code section loaded here
 *   DATA_BASE (0x2000): Data section loaded here
 *   DATA_BASE + data_size: BSS section (zeroed, bss_size bytes)
*/

#define PRG_HEADER_SIZE 16                          // Header is always 16 bytes
#define PRG_HEADER_OFFSET 0                         // Header starts at file offset 0
#define PRG_CODE_OFFSET PRG_HEADER_SIZE             // Code starts after header (offset 16)

/*
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

/**
 * .PRG FILE HEADER (16 btyes)
 */
typedef struct {
    uint8_t  magic[4];                              // File signature: "STYX" (0x53 0x54 0x59 0x58)                                                     │
    uint8_t  version;                               // Format version (currently 1)                                                                     │
    uint8_t  flags;                                 // Feature flags (currently 0 = none)                                                               │
    uint16_t entry_point;                           // PC initial value (must be CODE_BASE = 0x0100)                                                    │
    uint16_t code_size;                             // Code section size in bytes                                                                       │
    uint16_t data_size;                             // Data section size in bytes                                                                       │
    uint16_t bss_size;                              // BSS section size in bytes (NOT in file)                                                          │
    uint16_t reserved;                              // Reserved for future use (must be 0)
}PrgHeader_t;


#endif /* STYX_PRG_FORMAT_H */