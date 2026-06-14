/**
  * @file alu.c
  * @brief Arithmetic Logic Unit implementation
  */

#include "alu.h"
#include "../../common/reg_defs.h"
#include <stddef.h>


/**
  * Add two values
  * Output: a + b (16-bit with wraparound)
  * @return ERR_OK, or ERR_NULL_POINTER
 */
error_t alu_add(regval_t r1, regval_t r2, regval_t* result)
{
    if (result == NULL){
        return ERR_NULL_POINTER;
    }

    *result = (r1 + r2) & 0xFFFF;
    return ERR_OK;
}

/**
  * Subtract two values
  * Output: a - b (16-bit with wraparound)
  * @return ERR_OK, or ERR_NULL_POINTER
 */
error_t alu_sub(regval_t r1, regval_t r2, regval_t* result)
{
    if (result == NULL){
        return ERR_NULL_POINTER;
    }

    *result = (r1 - r2) & 0xFFFF;
    return ERR_OK;
}

/**
  * Multiply two values
  * Output: a * b (16-bit with wraparound)
  * @return ERR_OK, or ERR_NULL_POINTER
 */
error_t alu_mul(regval_t r1, regval_t r2, regval_t* result)
{
    if (result == NULL){
        return ERR_NULL_POINTER;
    }

    *result = (r1 * r2) & 0xFFFF;
    return ERR_OK;
}

/**
  * Divide two values
  * Output: r1 / r2 (integer division, quotient only)
  * @return ERR_OK, ERR_DIVIDE_BY_ZERO, or ERR_NULL_POINTER
  */
error_t alu_div(regval_t r1, regval_t r2, regval_t* result)
{
    if (result == NULL) {
        return ERR_NULL_POINTER;
    }

    if (r2 == 0) {
        *result = 0;
        return ERR_DIVISION_BY_ZERO;
    }

    *result = (r1 / r2) & 0xFFFF;
    return ERR_OK;
}

