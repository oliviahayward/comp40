/* Olivia Hayward
 * Claudia Aranda Barrios
 * uarray2.c
 * This is our implementation for UArray2. It contains all of the functions
 * needed to use the UArray2 interface.
 * 9/26/2020
 */

#include "uarray2.h"

#define U UArray2_T

struct U
{
    UArray_T array;
    int size;
    int height;
    int width;
};

/* function UArray2_new
 * input: int for the width, int for the height and int for the size of the
 * elements to be stored in the Uarray2
 * returns: UArray2_T type
 * does: it creates a new instance of the UArray2 struct specified with the
 * height and width and size of elements.
 * it would be a CRE if the height, width or size provided are non-positive.
 */
U UArray2_new(int width, int height, int size)
{
    UArray_T array_2d = UArray_new((height*width), size);

    U UArray2 = malloc(sizeof(struct U));

    UArray2->array = array_2d;
    UArray2->size = size;
    UArray2->height = height;
    UArray2->width = width;

    return UArray2;
}

/* function UArray2_free
 * input: pointer to an UArray2
 * returns: nothing
 * does: iterates through the array and frees the memory in each spot
 * it is a CRE for uarray2 or *uarray2 to be NULL
 */
void UArray2_free(U *uarray2)
{
    UArray_free(&(*uarray2)->array);
    free(*uarray2);
}

/* function UArray2_size
 * input: UArray2_T type whose size you want to know
 * returns: an integer
 * does: return the size of each element
 */
int UArray2_size(U uarray2)
{
    return uarray2->size;
}

/* function UArray2_width
 * input: UArray2_T type whose width you want to know
 * returns: an integer
 * does: return the width of the given UArray
 */
int UArray2_width(U uarray2)
{
    return uarray2->width;
}

/* function UArray2_height
 * input: UArray2_T type whose height you want to know
 * returns: an integer
 * does: return the height of the given UArray
 */
int UArray2_height(U uarray2)
{
    return uarray2->height;
}

/* function UArray2_at
 * input: UArray2_T type, int width and int height as coordinates
 * returns: a void pointer
 * does: return a pointer the value of the array at those coordinates
 * it would be a CRE if we pass coordinates that don't exist
 */
void *UArray2_at(U uarray2, int col, int row)
{
    void *element;
    
    assert(col <= uarray2->width && row <= uarray2->height);
    assert(col >= 0 && row >= 0);

    int width = uarray2->width;

    int position = (width * col) + row;
    element = UArray_at(uarray2->array, position);

    return element;
}

/* function UArray2_map_row_major
 * input: UArray2_T type, a pointer to a function, and a void pointer
 * returns: nothing
 * does: applies the given function to every elenent of the UArray, cols first
 * it would be a CRE if apply doesn't have the correct parameters
 */
void UArray2_map_row_major(U uarray2, void apply(int col, int row, U a, void *value, void *p2), void *cl)
{
    for (int row = 0; row < uarray2->height; row++) {
        for (int col = 0; col < uarray2->width; col++) {
            apply(col, row, uarray2, UArray2_at(uarray2, col, row), cl);
        }
    }
}

/* function UArray2_map_col_major
 * input: UArray2_T type, a pointer to a function, and a void pointer
 * returns: nothing
 * does: applies the given function to every elenent of the UArray, rows first
 * it would be a CRE if apply doesn't have the correct parameters
 */
void UArray2_map_col_major(U uarray2, void apply(int col, int row, U a, void *value, void *p2), void *cl)
{
    for (int col = 0; col < uarray2->width; col++) {
        for (int row = 0; row < uarray2->height; row++) {
            apply(col, row, uarray2, UArray2_at(uarray2, col, row), cl);
        }
    }
}