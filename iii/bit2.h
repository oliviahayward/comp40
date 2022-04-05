/* Olivia Hayward
Claudia Aranda Barrios
bit2.h
9/24/2020 */

#ifndef _BIT2ARRAY_H_
#define _BIT2ARRAY_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "bit.h"
#include "assert.h"

#define B Bit2_T
typedef struct B *B;


/* function Bit2_new
 * input: int for the width, int for the height
 * returns: bit2array_T type
 * does: it creates a new instance of the bit2array struct specified with the
 * height and width and size of elements.
 * it would be a CRE if the height, width or size provided are non-positive.
 */
B Bit2_new(int width, int height);

/* function Bit2_free
 * input: pointer to an bit2array
 * returns: nothing
 * does: iterates through the array and frees the memory in each spot
 * it is a CRE for bit2array or *bit2array to be NULL
 */
void Bit2_free(B *bitarray);

 /* function Bit2_width
  * input: bit2array_T type whose width you want to know
  * returns: an integer
  * does: return the width of the given bit array
  */
int Bit2_width(B bit2array);

/* function Bit2_height
 * input: bit2array_T type whose height you want to know
 * returns: an integer
 * does: return the height of the given bit array
 */
int Bit2_height(B bit2array);

/* function Bit2_get
 * input: bit2array_T type, int width and int height as coordinates
 * returns: an integer (either 0 or 1)
 * does: returns the value stored at those coordinates
 */
int Bit2_get(B bit2array, int col, int row);

/* function Bit2_put
 * input: bit2array_T type, int width and int height as coordinates, and an int
 * value of either 0 or 1
 * returns: nothing
 * does: changes the value at those coordinates with the given value
 */
int Bit2_put(B bit2array, int col, int row, int newValue);

/* function bit2array_map_row_major
 * input: bit2array_T type, a pointer to a function, and a void pointer
 * returns: nothing
 * does: applies the given function to every elenent of the bit2array, cols
 * first
 * it would be a CRE if apply doesn't have the correct parameters
 */
void Bit2_map_col_major(B bit2array, void apply(int col, int row, B a, int value, void *p2), void *cl);

/* function bit2array_map_col_major
 * input: bit2array_T type, a pointer to a function, and a void pointer
 * returns: nothing
 * does: applies the given function to every elenent of the bit2array, rows
 * first
 * it would be a CRE if apply doesn't have the correct parameters
 */
void Bit2_map_row_major(B bit2array, void apply(int col, int row, B a, int value, void *p2), void *cl);

void print_array(B bit2array);

#endif