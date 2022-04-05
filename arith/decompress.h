/* Olivia Hayward and Meghan Kloud
 * ohaywa02 and mkloud01
 * HW4 arith
 * decompress.h
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

/* Acts as the interface for decompression. See decompress.c for more
 * information.
 */
Seq_T decompress(uint64_t compressed);