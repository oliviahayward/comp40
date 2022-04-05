/* Olivia Hayward and Meghan Kloud
 * ohaywa02 and mkloud01
 * HW4 arith
 * imgManipulations.h
 * 10/26/2020
 */

#include "pnm.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "a2methods.h"
#include "seq.h"
#include <assert.h>
#include <stdlib.h>

/* Holds the blocksize, denominator, and a sequence of blocks */
struct Pixels {
    int blocksize;
    int denominator;
    Seq_T blocks;
};

/* If the image has an uneven number of rows or columns, it will crop the
 * image 
 */
Pnm_ppm crop(Pnm_ppm image, A2Methods_mapfun *map);

/* Iterates through a PPM image file and sets the PPM image file's Pnm_rgb
 * struct to the Pnm_rgb struct of the element being passed into the function
 */
void populate(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);

/* Takes a sequence of sequences and sets the element's Pnm_rgb struct
 * equal to the Pnm_rgb values of an element in currentSeq
 */
void convertSequence(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                               void *cl);
/* Converts a 2D array into a sequence of blocks */
void convertBlocked(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                              void *cl);

/* Frees all the codewords and then the sequence */
void freeCompressed(Seq_T compressedVals);

/* Frees all the nested sequences and then the larger sequence */
void free2DSequence(Seq_T newValues);