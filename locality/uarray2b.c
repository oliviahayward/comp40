/******************************************************************************
 *
 * Filename: uarray2b.c
 * By: Meghan Kloud (mkloud01) and Olivia Hayward (ohaywa02)
 * Date: 10/12/2020
 * Assignment: Homework 3 Part A: Block Arrays
 * Summary: This file contains the implementation for our 2D uarrayb data
 * structure. Read function contracts for more details.
 *
 *****************************************************************************/

#include "uarray2b.h"
#include "uarray.h"
#include <math.h>
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

#define T UArray2b_T

struct T {
    int width;
    int height;
    int size;
    int blocksize;
    int widthBuffer;
    int heightBuffer;
    int length;
    UArray_T array;
};

/* Function: UArray2b_new
 * Input: width, height, size of elements, blocksize
 * Returns: struct UArray2b_T array
 * Does: Creates a new instance of UArray2b_T with the specified information
 * Error cases: If the blocksize, width, height, or size are less than 1, exit
 *              the program. If malloc() returns NULL, exit the program.
 */
T UArray2b_new (int width, int height, int size, int blocksize)
{
    assert(blocksize >= 1);
    assert(width > 0);
    assert(height > 0);
    assert(size > 0);

    /* In order to read in the 2D array while keeping the blocksizes the same,
     * we created two buffer variables to calculate the empty space in each
     * block not taken up by the array's elements. */
    int widthBuffer = blocksize - (width % blocksize);
    int heightBuffer = blocksize - (height % blocksize);

    UArray_T fakeArray = UArray_new(((width + widthBuffer) *
                         (height + heightBuffer)), size);
    
    T newArray = malloc(sizeof(struct T));
    assert(newArray != NULL);

    newArray->array = fakeArray;
    newArray->width = width;
    newArray->widthBuffer = widthBuffer;
    newArray->heightBuffer = heightBuffer;
    newArray->height = height;
    newArray->size = size;
    newArray->blocksize = blocksize;
    newArray->length = (width + widthBuffer) * (height + heightBuffer);

    return newArray;
}

/* Function: UArray2b_new_64k_block
 * Input: width, height, size of elements
 * Returns: struct UArray2b_T array
 * Does: Creates a new instance of UArray2b_T array with the given
 *       specifications and an optimized blocksize
 * Error cases: If the width, height, or size is less than 1, exit the program.
 *              If malloc() returns NULL, exit the program.
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
    assert(width > 0);
    assert(height > 0);
    assert(size > 0);
    int blocksize;

    int totalNum = width * height;

    if (size > 65536) {
        blocksize = 1;
    } else if (totalNum * size < 65536) {
        double bsize = sqrt(totalNum);
        blocksize = ceil(bsize);
    } else if (totalNum * size > 65536){
        double numElems = 65536 / size;
        double bsize = sqrt(numElems);
        blocksize = bsize;
    }

    /* In order to read in the 2D array while keeping the blocksizes the same,
     * we created two buffer variables to calculate the empty space in each
     * block not taken up by the array's elements. */
    int widthBuffer = blocksize - (width % blocksize);
    int heightBuffer = blocksize - (height % blocksize);


    UArray_T fakeArray = UArray_new(((width + widthBuffer) *
                         (height + heightBuffer)), size);
    
    T newArray = malloc(sizeof(struct T));
    assert(newArray != NULL);

    newArray->array = fakeArray;
    newArray->width = width;
    newArray->widthBuffer = widthBuffer;
    newArray->heightBuffer = heightBuffer;
    newArray->height = height;
    newArray->size = size;
    newArray->blocksize = blocksize;
    newArray->length = (width + widthBuffer) * (height + heightBuffer);

    return newArray;
}

/* Function: UArray2b_free
 * Input: a pointer to a UArray2b_T array struct
 * Returns: nothing
 * Does: Frees the memory allocated to that struct and the pointer
 * Error cases: If the struct is NULL or if the pointer to the struct is NULL,
 *              exit the program.
 */
void UArray2b_free (T *array2b)
{
    assert(array2b != NULL && *array2b != NULL);

    UArray_free(&(*array2b)->array);
    free(*array2b);
}

/* Function: UArray2b_width
 * Input: UArray2b_T array struct
 * Returns: the width of the 2D array
 * Does: Returns the width of the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
int UArray2b_width (T array2b)
{
    assert(array2b != NULL);
    return array2b->width;
}

/* Function: UArray2b_height
 * Input: UArray2b_T array struct
 * Returns: the height of the 2D array
 * Does: Returns the height of the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
int UArray2b_height (T array2b)
{
    assert(array2b != NULL);
    return array2b->height;
}

/* Function: UArray2b_size
 * Input: UArray2b_T array struct
 * Returns: the size of the elements in the 2D array
 * Does: Returns the size of the elements in the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
int UArray2b_size (T array2b)
{
    assert(array2b != NULL);
    return array2b->size;
}

/* Function: UArray2b_blocksize
 * Input: UArray2b_T array struct
 * Returns: the blocksize of the 2D array
 * Does: Returns the blocksize of the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
int UArray2b_blocksize (T array2b)
{
    assert(array2b != NULL);
    return array2b->blocksize;
}

/* Function: UArray2b_length
 * Input: UArray2b_T array struct
 * Returns: the length of the 2D array
 * Does: Returns the length of the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
int UArray2b_length (T array2b)
{
    assert(array2b != NULL);
    return array2b->length;
}

/* Function: UArray2b_at
 * Input: UArray2b_T array struct, column number, and row number
 * Returns: A void pointer to the element at that coordinate in the array
 * Does: It calculates the number of blocks per row and gets the block
 *       coordinates. It uses that to get the index of the block that the
 *       coordinate is located in. It uses that to calculate the 
 *       beginning index of that block and then uses modulo arithmetic to 
 *       determine the index of the element.
 * Error cases: If the UArray2b_T struct is NULL, exit the program. If the
 *              column or row number is less than 0 or greater than the
 *              dimensions of the image, exit the program. If the calculated
 *              index is larger than the length of the array or is less than
 *              0, exit the program.
 */
void *UArray2b_at(T array2b, int column, int row)
{
    assert(array2b != NULL);
    assert(column >= 0 && column < array2b->width);
    assert(row >= 0 && row < array2b->height);

    int blocksize = array2b->blocksize;
    int blocksPerRow = ceil((double)(array2b->width) / (double)blocksize);

    /* Finds the coordinates of the block */
    int blockX = floor(row / blocksize);
    int blockY = floor(column / blocksize);

    /* Calculates the block "number" using its coordinates */
    int blockNum = blockX * blocksPerRow + blockY + 1;

    /* Gets the index of the first element in the block */
    int indexOfFirst = (blockNum - 1) * (blocksize * blocksize);

    int x = column % blocksize;
    int y = row % blocksize;

    /* Gets the index of the element we are looking for */
    int index = indexOfFirst + x + (blocksize * y);

    assert(index < UArray2b_length(array2b));
    assert(index >= 0);

    return UArray_at(array2b->array, index);
}

/* Function: UArray2b_map
 * Input: UArray2b_T array struct, apply function, void pointer to a closure
 *        argument
 * Returns: Nothing
 * Does: Iterates throught the array and applies the apply function to every
 *       element in the array
 * Error cases: If the struct, the struct's array, or the apply function is
 *              NULL, exit the program.
 */
void UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                  void *elem, void *cl), void *cl)
{
    assert(array2b != NULL);
    assert(array2b->array != NULL);
    assert(apply != NULL);

    /* Iterating through the blocks */
    for(int y = 0; y < UArray2b_height(array2b); y += array2b->blocksize) {
        for(int x = 0; x < UArray2b_width(array2b); x += array2b->blocksize) {
            
            /* Iterating through the elements in each block */
            for (int i = y; i < y + array2b->blocksize; i++) {
                for (int j = x; j < x + array2b->blocksize; j++) {
                    
                    if (j >= UArray2b_width(array2b) ||
                        i >= UArray2b_height(array2b)) {
                        continue;
                    } else {
                        apply(j, i, array2b, UArray2b_at(array2b, j, i), cl);
                    }
                }
            }
        }
    }
}