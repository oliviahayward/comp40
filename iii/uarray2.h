/* Olivia Hayward
 * Claudia Aranda Barrios
 * uarray2.h
 * This is the header file for UArray2 implementation
 * 9/24/2020
 */

#ifndef _UARRAY2_H_
#define _UARRAY2_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "uarray.h"
#include "assert.h"

#define U UArray2_T
typedef struct U *U;

/* Creates a new instance of the UArray2 */
U UArray2_new(int width, int height, int size);

/* Iterates through the array and frees the memory in each spot */
void UArray2_free(U *uarray2);

/* Returns the size of each element */
 int UArray2_size(U uarray2);

/* Returns the width of the given UArray */
int UArray2_width(U uarray2);

/* Returns the height of the given UArray */
int UArray2_height(U uarray2);

/* Returns a pointer the value of the array at those coordinates */
void *UArray2_at(U uarray2, int width, int height);

/* Applies the given function to every elenent of the UArray, columns first */
void UArray2_map_row_major(U uarray2, void apply(int i, int j, U a,
                                void *value, void *p2), void *cl);

/* Applies the given function to every elenent of the UArray, rows first */
void UArray2_map_col_major(U uarray2, void apply(int i, int j, U a,
                                void *value, void *p2), void *cl);

#undef U
#endif