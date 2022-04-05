/* Olivia Hayward
 * Meghan Kloud
 * HW4 arith
 * bitpack.c
 * 10/25/2020
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "math.h"
#include "bitpack.h"
#include <stdbool.h>

/* Function: Bitpack_fitsu
 * Input: an unsigned number n and a width 
 * Does: Checks that the number will fit into a space in memory with the
 *       specified width
 * Returns: a bool
 * Error checks: None
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    uint64_t limit = ((uint64_t)1 << width) - 1;

    if (limit >= n) {
        return true;
    }
    else {
        return false;
    }
}

/* Function: Bitpack_fitss
 * Input: a signed number n and a width 
 * Does: Checks that the number will fit into a space in memory with the
 *       specified width
 * Returns: a bool
 * Error checks: None
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    int64_t limit = ((uint64_t)1 << (width - 1)) - 1;

    if (limit >= n) {
        return true;
    }
    else {
        return false;
    }
}

/* Function: Bitpack_getu
 * Input: A 32-bit codeword, width, and least significant bit
 * Does: Returns the unsigned value at that location in the codeword
 * Returns: the unsigned value at a specified location in the codeword
 * Error checks: If the width is less than 0 or larger than 64, it is a CRE.
 *               If the sum of the width and the lsb is larger than 64, it's a
 *               CRE.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert((width + lsb) <= 64);

    /* setting the mask */
    uint64_t mask = ~0;
    mask = mask >> (64 - width);
    mask = mask << lsb;
    uint64_t value = word & mask;
    value = value >> lsb;

    return value;
}

/* Function: Bitpack_gets
 * Input: A 32-bit codeword, width, and least significant bit
 * Does: Returns the signed value at that location in the codeword
 * Returns: the signed value at a specified location in the codeword
 * Error checks: If the width is less than 0 or larger than 64, it is a CRE.
 *               If the sum of the width and the lsb is larger than 64, it's a
 *               CRE.
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert((width + lsb) <= 64);

    /* setting the mask */
    uint64_t mask = ~0;
    mask = mask >> (64 - width) << lsb;
    int64_t value = word & mask;
    value = value >> lsb;

    /* checking to see if the sign is negative or positive */
    uint64_t value2 = Bitpack_getu(word, 1, lsb + width -1);
    
    if (value2 == 1) {
        uint64_t mask2 = ~0;
        mask2 = mask2 >> (64 - width);
        mask2 = ~mask2;
        value = value | mask2;
    }

    return value;
}

/* Function: Bitpack_newu
 * Input: A 32-bit codeword, width, least significant bit, and the unsigned
 *        value
 * Does: Adds an unsigned value into the codeword at the specified location
 * Returns: The updated codeword
 * Error checks: If the width is less than 0 or larger than 64, it is a CRE.
 *               If the sum of the width and the lsb is larger than 64, it's a
 *               CRE.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                                                   uint64_t value)
{
    assert(width <= 64);
    assert((width + lsb) <= 64);
    assert(Bitpack_fitsu(value, width));
    value = value << lsb;

    /* setting the mask */
    uint64_t mask = ~0;
    mask = mask >> (64 - width) << lsb;
    mask = ~mask;
    uint64_t newWord = mask & word;
    uint64_t finalWord = value | newWord;

    return finalWord;
}

/* Function: Bitpack_news
 * Input: A 32-bit codeword, width, least significant bit, and the signed
 *        value
 * Does: Adds an signed value into the codeword at the specified location
 * Returns: The updated codeword
 * Error checks: If the width is less than 0 or larger than 64, it is a CRE.
 *               If the sum of the width and the lsb is larger than 64, it's a
 *               CRE.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                                                    int64_t value)
{
    assert(width <= 64);
    assert((width + lsb) <= 64);
    assert(Bitpack_fitss(value, width));
    value = value << (64 - width);
    value = (uint64_t) value >> (64 - width - lsb);

    /* setting the mask */
    uint64_t mask = ~0;
    mask = mask >> (64 - width + 1) << lsb;
    mask = ~mask;
    uint64_t newWord = mask & word;
    uint64_t finalWord = value | newWord;
    
    return finalWord;
}