  /**
    * @file alu.h
    * @brief Arithmetic Logic Unit - Public API
    *
    * Provides arithmetic operations and comparison
    * All functions are pure computation - no side effects on registers.
   */

#ifndef ALU_H
#define ALU_H

#include "common/types.h"
#include "util/errors.h"


error_t alu_add(regval_t r1, regval_t r2, regval_t* result);
error_t alu_sub(regval_t r1, regval_t r2, regval_t* result);
error_t alu_mul(regval_t r1, regval_t r2, regval_t* result);
error_t alu_div(regval_t r1, regval_t r2, regval_t* result);

/**
  * Compare two values (for CMP instruction)
  * Computes r1 - r2 and sets FLAGS accordingly
  *
  * @param r1 First operand
  * @param r2 Second operand
  * @param flags Output: FLAGS register value (Z, N, C, V bits set)
  * @return ERR_OK, or ERR_NULL_POINTER
*/
error_t alu_compare(regval_t r1, regval_t r2, flags_t* flags);
#endif // ALU_H