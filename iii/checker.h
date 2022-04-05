/* Olivia Hayward
* Claudia Aranda Barrios
* HW2
* checker.h
* 10/1/2020
* This file contains the interface for checker
*/

#ifndef _CHECKER_H_
#define _CHECKER_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "uarray2.h"
#include "assert.h"
#include "pnmrdr.h"


typedef struct Pnmrdr_T *T;

/* opens the given file */
FILE *openFile(int argc, char *argv[]);

/* transforms the pgm file data into an UArray2 */
UArray2_T pgmToArray(FILE* filename);

/* transforms the pgm file data into an UArray2 */
UArray2_T populateBoard(Pnmrdr_T reader, Pnmrdr_mapdata mapdata);

/* calls the three functions that check the sudoku by row, col and blocks */
void checkSolution(UArray2_T board);

/* checks that there are no repeats by column in the sudoku board */
void checkCols(UArray2_T board);

/* checks that there are no repeats by row in the sudoku board */
void checkRows(UArray2_T board);

/* calls the function that checks a single block on all of the blocks */
void checkBlocks(UArray2_T board);

/* checks that there are no repeats by blocks in the sudoku board */
void check3by3(UArray2_T board, int colStart, int colEnd,
                int rowStart, int rowEnd);

/* adds numbers to an array of size 9, and exits if it was already there */
void addToArray(int values[], int entry);

#endif