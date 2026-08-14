#include "memory.h"
#include "common/prg_format.h"
#include "common/mem_map.h"
#include "util/errors.h"

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

//* Internal helper: Write code/data/bss section to memory
static error_t load_sections_to_memory(
    Memory_t* mem,
    const uint8_t* code_bytes,
    size_t code_size,
    const uint8_t* data_bytes,
    size_t data_size,
    size_t bss_size
) {
    // Set memory to LOADING mode
    mem_set_protection(mem, MEM_LOAD_MODE);

    // Load Code section to 0x0100
    for (size_t i = 0; i < code_size; i++) {
        mem_write_byte(mem, MEM_CODE_START + i, code_bytes[i]);
    }

    // 3. Load DATA section to 0x2000 (if provided)
    if (data_bytes != NULL && data_size > 0) {
        for (size_t i = 0; i < data_size; i++) {
            mem_write_byte(mem, MEM_DATA_BASE + i, data_bytes[i]);
        }
    }

    // Zero BSS section
    if (bss_size > 0) {
        addr_t bss_start = MEM_DATA_BASE + data_size;
        for (size_t i = 0; i < bss_size; i++) {
            mem_write_byte(mem, bss_start + i, 0x00);
        }
    }

    // 5. Save statistics
    mem_set_usage(mem, code_size, data_size, bss_size);

    // 6. Set memory to EXECUTE mode
    mem_set_protection(mem, MEM_EXECUTE_MODE);

    return ERR_OK;
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

    // Read CODE section into buffer
    uint8_t code_buffer[MEM_CODE_SIZE];  // Temp buffer
    if (fread(code_buffer, 1, header.code_size, fp) != header.code_size) {
        fclose(fp);
        return ERR_FILE_READ;
    }

    // Read DATA section into buffer (if exists)
    uint8_t data_buffer[MEM_DATA_SIZE];  // Temp buffer
    uint8_t* data_ptr = NULL;
    if (header.data_size > 0) {
        if (fread(data_buffer, 1, header.data_size, fp) != header.data_size) {
            fclose(fp);
            return ERR_FILE_READ;
        }
        data_ptr = data_buffer;
    }

    fclose(fp);

    return load_sections_to_memory(
        mem,
        code_buffer, header.code_size,
        data_ptr, header.data_size,
        header.bss_size
    );
}

//* Load program from memory buffers (no file I/O)
error_t mem_load_from_bytes(
    Memory_t* mem,
    const uint8_t* code_bytes,
    size_t code_size,
    const uint8_t* data_bytes,
    size_t data_size
) {
    return load_sections_to_memory(
        mem,
        code_bytes, code_size,
        data_bytes, data_size,
        0  // BSS = 0
    );
}