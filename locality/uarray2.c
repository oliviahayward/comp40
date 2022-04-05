/******************************************************************************
*
* Filename: uarray2.c
* By: Meghan Kloud (mkloud01) and Noah Wright (nwrigh05)
* Date: 10/1/2020
* Assignment: Homework 2 Part A: Two-Dimensional, Polymorphic, Unboxed Arrays
* Summary: This file contains the implementation for our 2D uarray data
* structure. Read function contracts for more details.
*
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "uarray.h"
#include "uarray2.h"
#include <assert.h>
#include "mem.h"

#define T UArray2_T

/* This struct is used to store the width and height of the 2D uarray. The
   2D array elements are stored in UArray_T uarray. */
struct T {
    int width;
    int height;
    UArray_T uarray;
};

/* Creates a new uarray2 object */
T UArray2_new(int width, int height, int size)
{
    assert((width > 0 && height > 0) || (width == 0 && height == 0));
    assert (size > 0);

    T uarray2;
    NEW(uarray2);

    int totalLength = width * height;

    uarray2->uarray = UArray_new(totalLength, size);
    uarray2->width = width;
    uarray2->height = height;

    return uarray2;
}

/* Frees the memory in the givin uarray2 object */
void UArray2_free(T *uarray2)
{
    assert(uarray2 != NULL && *uarray2 != NULL);

    UArray_free(&(*uarray2)->uarray);
    free(*uarray2);
}

/* Returns uarray2 width */
int UArray2_width(T uarray2) {
    assert(uarray2 != NULL);
    return uarray2->width;
}

/* Returns the uarray2 height */
int UArray2_height(T  uarray2) {
    assert(uarray2 != NULL);
    return uarray2->height;
}

/* Returns the uarray2 size */
int UArray2_size(T uarray2) {
    assert(uarray2 != NULL);
    return UArray_size(uarray2->uarray);
}

/* Returns a void pointer to the element at x,y in the given uarray2 */
void *UArray2_at(T uarray2, int x, int y)
{
    assert(uarray2 != NULL);
    assert(x >= 0 && x < uarray2->width);
    assert(y >=0 && y < uarray2->height);

    int index = uarray2->height * x + y;

    return UArray_at(uarray2->uarray, index);
}

/* Traverses the uarray2 left to right, top to bottom and runs apply on each
   element */
void UArray2_map_row_major(T uarray2, void apply(int x, int y,
                                   T uarray2, void *value, void *cl), void *cl)
{
    assert(uarray2 != NULL);
    assert(uarray2->uarray != NULL);
    assert(apply != NULL);

    for(int y = 0; y < uarray2->height; y++) {
        for(int x = 0; x < uarray2->width; x++) {
            apply(x, y, uarray2, UArray2_at(uarray2, x, y), cl);
        }
    }
}

/* Traverses the uarray2 top to bottom, left to right and runs apply on each
   element */
void UArray2_map_col_major(T uarray2, void apply(int x, int y,
                           T uarray2, void *value, void *cl), void *cl)
{   
    assert(uarray2 != NULL);
    assert(uarray2->uarray != NULL);
    assert(apply != NULL);
    for(int x = 0; x < uarray2->width; x++) {
        for(int y = 0; y < uarray2->height; y++) {
            apply(x, y, uarray2, UArray2_at(uarray2, x, y), cl);
        }
    }
}

/* Traverses the uarray2 block by block, top to bottom, left to right in each
   block and runs apply on each element */
void UArray2_map_block_major(T uarray2, void apply(int x, int y,
                             T uarray2, void *value, void *cl), void *cl)
{
    assert(uarray2 != NULL);
    assert(uarray2->uarray != NULL);
    assert(apply != NULL);
    for(int x = 0; x < uarray2->width; x += 3) {
        for(int y = 0; y < uarray2->height; y += 3) {
            for (int i = x; i < x + 3; i++) {
                for (int j = y; j < y + 3; j++) {
                    apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
                }
            }
        }
    }
}