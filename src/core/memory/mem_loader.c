#include "memory.h"
#include "../../common/prg_format.h"
#include "../../common/mem_map.h"
#include "../../utils/errors.h"

#include <stdio.h>
#include <string.h>

/*
    Validate .prg header file before loading
*/
static error_t validate_prg_header(const PrgHeader_t *header)
{

}

/*
    * Main Loader function
*/
error_t mem_load_program(Memory_t *mem, const char *filename)
{
    
}
