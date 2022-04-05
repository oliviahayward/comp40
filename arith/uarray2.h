/******************************************************************************
*
* Filename: uarray2.h
* By: Meghan Kloud (mkloud01) and Noah Wright (nwrigh05)
* Date: 10/1/2020
* Assignment: Homework 2 Part A: Two-Dimentional, Polymorphic, Unboxed Arrays
* Summary: This file contains the interface for out 2D uarray data structure.
*
******************************************************************************/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;

/* This function will create a new uarray2 object of the specified width and
   height that holds values of the specified size and return the new uarray2.
   The height, width, and size cannot be less than zero. */
extern T     UArray2_new           (int width, int height, int size);

/* This function will deallocate the memory associated with the passed uarray2.
   The uarray2 given cannot be NULL or else it will result in a checked runtime
   error. */
extern void  UArray2_free          (T *uarray2);

/* UArray2_width will return the width of a given uarray2. Will result in a
   checked runtime error if the passed uarray2 is NULL. */
extern int   UArray2_width         (T  uarray2);

/* UArray2_height will return the height of a given uarray2. Will result in a
   checked runtime error if the passed uarray2 is NULL. */
extern int   UArray2_height        (T  uarray2);

/* UArray2_size will return the size of a given uarray2. Will result in a
   checked runtime error if the passed uarray2 is NULL. */
extern int   UArray2_size          (T  uarray2);

/* Returns a pointer to the index x,y in uarray2. Results in a checked runtime
   error if x >= width, or y >= height. Results in a checked runtime error if
   uarray2 is NULL. */
extern void *UArray2_at            (T  uarray2, int x, int y);

/* This function traverses the passed uarray2 in row major order (reading the
 *  2D array from left to right, top to bottom) and performs the apply function
 *  on each element. The apply function is passed the uarray2, the current x,y
 *  index, the corresponding value, and a cl pointer. The apply function passed
 *  must have the same arguments specified in this declaration. The cl argument
 *  will be an object passed by reference to the function. Function will result
 *  in a checked runtime error if a NULL uarray2 or apply function is passed.
 */
extern void UArray2_map_row_major  (T uarray2, void apply(int x, int y,
                                    T uarray2, void *value, void *cl),
                                    void *cl);

/* This function traverses the passed uarray2 in column major order (reading
 * the 2D array from top to bottom, left to right) and performs the apply
 * function on each element. The apply function is passed the uarray2, the
 * current x,y index, the correspoinding value, and a cl pointer. The apply
 * function passed must have the same arguments specified in this declaration.
 * The cl argument will be an object passed by reference to the function.
 * Function will result in a checked runtime error if a NULL uarray2 is passed.
 */
extern void UArray2_map_col_major  (T uarray2, void apply(int x, int y,
                                    T uarray2, void *value, void *cl),
                                    void *cl);
/*
 * This function traverses the passed uarray2 in block major order (reading
 * the 2D array in 3x3 blocks from top to bottom, left to right) and
 * performs the apply function on each element. The apply function is passed
 * the uarray2, the current x,y index, the correspoinding value, and a cl
 * pointer. The apply function passed must have the same arguments specified
 * in this declaration. The cl argument will be an object passed by reference
 * to the function. Function will result in a checked runtime error if a NULL
 * uarray2 or apply function is passed. By reading the uarray in 3x3 blocks,
 * one can check the result of multiple games being represented by the 2D array
 * such as Sudoku or Tic-Tac-Toe.
 */
extern void UArray2_map_block_major(T uarray2, void apply(int x, int y,
                                    T uarray2, void *value, void *cl),
                                    void *cl);

#undef T

#endif