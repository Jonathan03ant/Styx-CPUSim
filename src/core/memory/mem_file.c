/**
  * @file mem_file.c
  * @brief Memory array implementation - 64KB byte-addressable memory
  */

#include "memory.h"
#include "../../common/mem_map.h"
#include "../../utils/bitops.h"
#include <stdlib.h>
#include <string.h>



/**
 * Memory Subsystem structure
 * 64kb of array and protection mode
 */
struct Memory_s {
    byte_t data[MEM_TOTAL_SIZE];                // 64KB memory array (0x0000 - 0xFFFF)
    MemMode_e mode;                             // Protection mode (LOADING or RUNNING)
};


/**
 * Create and initialize memory subsystem (one time)
 * Allocates 64kb memory array and initialize it to zero
 * @return Pointer to Memory Object
 */
Memory_t* mem_create(void)
{
    Memory_t *mem = (Memory_t*)malloc(sizeof(Memory_t));
    if (mem == NULL) {
        return NULL;
    }
    mem_init(mem);
    return mem;
}

error_t mem_init(Memory_t *mem)
{
    if (mem == NULL){
        return ERR_NULL_POINTER;
    }

    memset(mem->data, 0, MEM_TOTAL_SIZE);
    mem->mode = MEM_LOAD_MODE;
    return ERR_OK;
}

void mem_destroy(Memory_t *mem)
{
    if (mem != NULL){
        free(mem);
    }
}

error_t mem_reset(Memory_t *mem){
    // Rest == Reinitialize
    return mem_init(mem);
}

/**
 * Read mem address 0xXXXX
 * @return populate mem_address with value
 */
error_t mem_read_byte(Memory_t *mem, addr_t addr, byte_t *value){
    if (mem == NULL || value == NULL) {
        return ERR_NULL_POINTER;
    }

    // addr_t is uint16_t,  0x0000-0xFFFF
    *value = mem->data[addr];
    return ERR_OK;
}

/**
 * Write 8 bit value to a memory address 0xXXXX
 */
error_t mem_write_byte(Memory_t *mem, addr_t addr, byte_t value)
{
    if (mem == NULL) {
        return ERR_NULL_POINTER;
    }

    // Check code region protection
    if (mem->mode == MEM_EXECUTE_MODE){
        if(addr >= MEM_CODE_BASE && addr <= MEM_CODE_END){
            return ERR_WRITE_PROTECTED;
        }
    }

    mem->data[addr] = value;
    return ERR_OK;
}