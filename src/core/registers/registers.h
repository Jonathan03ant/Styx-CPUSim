#ifndef STYX_REGISTERS_H
#define STYX_REGISTERS_H

#include "../../common/types.h"
#include "../../common/reg_defs.h"
#include "../../utils/errors.h"

/*----------------------------------------------------------------------------
  * REGISTER FILE = PUBLIC API
  *----------------------------------------------------------------------------*/
typedef struct RegisterFile_s RegisterFile_t;

RegisterFile_t reg_create(void); // crate register file
void reg_init(RegisterFile_t *rf);
void reg_destroy(RegisterFile_t *rf);
void reg_reset(RegisterFile_t *rf);


#endif /* STYX_REGISTERS_H */