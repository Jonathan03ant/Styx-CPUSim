#include "memory.h"
#include "../../common/prg_format.h"
#include "../../common/mem_map.h"
#include "../../utils/errors.h"

#include <stdio.h>
#include <string.h>

//* Validate .prg header file before loading
static error_t validate_prg_header(const PrgHeader_t *header)
{
    /*
        * Magic number check, version check
        * Entry point/codesize/datasize validation
    */

    // Magic number check
    if (header->magic[0] != 'S' ||
        header->magic[1] != 'T' ||
        header->magic[2] != 'Y' ||
        header->magic[3] != 'X'){
        return ERR_INVALID_PRG_FORMAT;
    }
    //Version check
    if (header->version != PRG_VERSION_1) {
        return ERR_UNSUPPORTED_PRG_VERSION;
    }
    // Entry point validation
    if (header->entry_point != MEM_CODE_START) {
        return ERR_INVALID_ENTRY_POINT;
    }
    // Code size validation (fits in CODE region?)
    if (header->code_size > MEM_CODE_SIZE) {
        return ERR_CODE_TOO_LARGE;
    }
    // Data size validation (fits in DATA region?)
    if (header->data_size > MEM_DATA_SIZE) {
        return ERR_DATA_TOO_LARGE;
    }
    // BSS size validation (fits in remaining DATA/HEAP?)
    uint32_t total_data = header->data_size + header->bss_size;
    if (total_data > (MEM_DATA_SIZE + MEM_HEAP_SIZE)) {
        return ERR_BSS_TOO_LARGE;
    }
    // Reserved field check (must be 0)
    if (header->reserved != 0) {
        return ERR_INVALID_PRG_FORMAT;
    }

    return ERR_OK;  // Valid!
}

//* Main Loader function

error_t mem_load_program(Memory_t *mem, const char *filename)
{
    /*
        * read/validate header
        * Set memory to loading
        * Load CODE/DATA section
    */

    // Open file and read header
    FILE *fp = fopen(filename, "rb");
    if (!fp){
        return ERR_FILE_NOT_FOUND;
    }

    PrgHeader_t header;
    if (fread(&header, sizeof(PrgHeader_t), 1, fp) != 1){
        fclose(fp);
        return ERR_FILE_READ;
    }

    // validate header
    error_t err = validate_prg_header(&header);
    if (err != ERR_OK) {
        fclose(fp);
        return err;  // Invalid header
    }

    // Set memory to loading mode (code writable)
    mem_set_protection(mem, MEM_LOAD_MODE);

    // Load code section to MEM_CODE_START(0x0100)
    for (uint16_t i = 0; i < header.code_size; i++){
        uint8_t byte;
        if (fread(&byte, 1, 1, fp) != 1){
            fclose(fp);
            return ERR_FILE_READ;
        }
        mem_write_byte(mem, MEM_CODE_START + i, byte);
    }

    // Load data section to MEM_DATA_BASE (0x2000)
    for (uint16_t i = 0; i < header.data_size; i++){
        uint8_t byte;
        if (fread(&byte, 1, 1, fp) != 1){
            fclose(fp);
            return ERR_FILE_READ;
        }
        mem_write_byte(mem, MEM_DATA_BASE + i, byte);
    }

    // Zero BSS section (MEM_DATA_BASE + data_size)
    addr_t bss_start = MEM_DATA_BASE + header.data_size;
    for (uint16_t i = 0; i < header.bss_size; i++) {
        mem_write_byte(mem, bss_start + i, 0x00);
    }

    // Set memory to execute mode
    mem_set_protection(mem, MEM_EXECUTE_MODE);
    fclose(fp);
    return ERR_OK;
}
