/**
  * @file alu.c
  * @brief Arithmetic Logic Unit implementation
  */

#include "alu.h"
#include "../../common/reg_defs.h"
#include <stddef.h>
#include <stdbool.h>


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

/**
  * Helper function for flags (static)
  */
static flags_t calc_flags(regval_t r1, regval_t r2, regval_t result)
{
    flags_t flags = 0;

    // Z flag: result is zero
    if (result == 0){
        flags |= FLAG_Z;
    }

    // N flag: result is negative
    if (result & 0x8000) {
        flags |= FLAG_N;
    }

    // C flag: unsigned overflow (r1 < r2 for subtraction)
    if (r1 < r2) {
        flags |= FLAG_C;
    }

    // V flag: signed overflow
    // Overflow occurs when:
    //  - Subtracting negative from positive gives negative result
    //  - Subtracting positive from negative gives positive result
    bool r1_negative = (r1 & 0x8000) != 0;
    bool r2_negative = (r2 & 0x8000) != 0;
    bool result_negative = (result & 0x8000) != 0;

    if ((r1_negative != r2_negative) && (result_negative == r2_negative)) {
        flags |= FLAG_V;
    }

    return flags;
}

/**
  * Compare two values (for CMP instruction)
  * Computes r1 - r2 and sets FLAGS accordingly
  * Output: FLAGS register value (Z, N, C, V bits set)
  * @return ERR_OK, or ERR_NULL_POINTER
 */
error_t alu_compare(regval_t r1, regval_t r2, flags_t *flags) {
    if (flags == NULL) {
        return ERR_NULL_POINTER;
    }

    // Compute subtraction (r1 - r2)
    regval_t result = (r1 - r2) & 0xFFFF;

    // Calculate all FLAGS based on result
    *flags = calc_flags(r1, r2, result);

    return ERR_OK;
}

  