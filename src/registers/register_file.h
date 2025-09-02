/*
    * Public API Intreface for registers
*/
#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include "register_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Register File Initialization
void register_file_init(void);
void register_file_reset(void);

//General Register Operations
uint16_t register_file_read_general(GeneralRegisterID regID);
void register_file_write_general(GeneralRegisterID regID, uint16_t value);
uint16_t *register_file_get_general_array(void);

//Special Registers Operations
uint16_t register_file_read_special(SpecialRegisterID regID);
void register_file_write_special(SpecialRegisterID regID, uint16_t value);

//Context Mgmt, save/restore register state
void register_file_save_context(CPURegisterFile *context);
void register_file_load_context(CPURegisterFile *context);

//Utility functions (will be populated more)
void register_file_dump(void);
void register_file_dump_general(void);
void register_file_dump_special(void);
bool register_file_parse_general_reg(uint8_t reg_bits, GeneralRegisterID *output);













#ifdef __cplusplus
}
#endif

#endif // REGISTER_FILE_H