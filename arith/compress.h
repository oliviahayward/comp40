/* Olivia Hayward and Meghan Kloud
 * ohaywa02 and mkloud01
 * HW4 arith
 * compress.h
 * 10/26/2020
 */

#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "seq.h"
#include "pnm.h"
#include "arith40.h"
#include "assert.h"
#include "bitpack.h"

/* Acts as the interface for compression. See compress.c for more information.
 */
uint64_t compress(Seq_T block, int denominator);