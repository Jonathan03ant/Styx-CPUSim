#ifndef STYX_BITOPS_H
#define STYX_BITOPS_H

#include <stdint.h>
#include <stdbool.h>
#include "../common/types.h"


static inline bool test_bit(uint16_t value, int bit_pos)
{
    return value & (1 << bit_pos);
}

static inline uint16_t set_bit(uint16_t value, int bit_pos)
{
    return value | (1 << bit_pos);
}

static inline uint16_t clear_bit(uint16_t value, int bit_pos)
{
    return value & ~(1 << bit_pos);
}

static inline uint8_t get_low_byte(uint16_t word)
{
    return word & 0xFF;
}

static inline uint8_t get_high_byte(uint16_t word)
{
    return (word >> 8) & 0xFF;
}

static inline uint16_t make_word(uint8_t high, uint8_t low)
{

    uint16_t high_shifted = (uint16_t) high << 8;
    uint16_t word = high_shifted | low;
    return word;
}












 #endif /* STYX_BITOPS_H */