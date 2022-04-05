/* Olivia Hayward
 * Claudia Aranda Barrios
 * HW2
 * checker.c
 * 10/1/2020
 *
 * This file contains the implementation of checker, which is the solution
 * checker for a given sudoku.
 */

#include "checker.h"

/* function openFile
 * input: the number of arguments and the name of the file
 * returns: pointer to the open file if given in command line
 * does: opens the file given in command line or asks for it on standard input
 * error cases: if the file is not correctly opened exit the program
 */
FILE *openFile(int argc, char *argv[])
{
    FILE *file;
    if (argc == 2) {
        if (argv[1] == NULL) {
            exit(1);
        }
        
        char *filename = argv[1];
        
        file = fopen(filename, "r");
        
        if (file == NULL) {
            exit(1);
        }
    }
    
    else {
        file = stdin;
    }

    return file;
}

/* function pgmToArray
 * input: pointer to a file we want to read
 * returns: an UArray_2 filled with the potential solution in the pgm file
 * does: creates a reader for the pnm file and calls the function to fill
 * a UArray2 with that data
 * error cases: if the file is not correctly formatted raise an assertion
 */
UArray2_T pgmToArray(FILE* filename)
{
    Pnmrdr_T reader = Pnmrdr_new(filename);
    Pnmrdr_mapdata mapdata = Pnmrdr_data(reader);

    UArray2_T sudokuBoard = populateBoard(reader, mapdata);

    Pnmrdr_free(&reader);
    return sudokuBoard;
}

/* function populateBoard
 * input: a pnm reader and mapdata from a pnm file
 * returns: an UArray_2 filled with the potential solution in the pgm file
 * does: populates a UArray_2 with the data from the mapdata
 * error cases: if the size is not 9x9 or the type is not a pgm, exits program
 */
UArray2_T populateBoard(Pnmrdr_T reader, Pnmrdr_mapdata mapdata)
{
    int width = mapdata.width;
    int height = mapdata.height;
    int type = mapdata.type;
    
    if (width != 9 || height != 9) {
        exit(1);
    }

    if (type != Pnmrdr_gray) {
        exit(1);
    }

    int sudokuValue;
    UArray2_T sudokuBoard = UArray2_new(width, height, sizeof(int));

    /* populates the sudoku board row major*/
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            sudokuValue = Pnmrdr_get(reader);
            *((int *)UArray2_at(sudokuBoard, col, row)) = sudokuValue;
        }
    }

    return sudokuBoard;
}

/* function checkSolution
 * input: the sudoku board with the numbers filled
 * returns: nothing
 * does: calls the three functions to check if there are any repeats in the
 * rows, columns or blocks
 */
void checkSolution(UArray2_T board)
{
    checkCols(board);
    checkRows(board);
    checkBlocks(board);
}

/* function checkCols
 * input: the sudoku board with the numbers filled
 * returns: nothing
 * does: goes through board by the columns, and adds the numbers to an array
 * of size 9 initialized with only -1 with the function addToArray to check
 * for repeated numbers
 * error cases: if addToArray detects a repeat, the program exits. if a number
 * on the sudoku board was not between 1 or 9 the program exits.
 */
void checkCols(UArray2_T board)
{
    for (int col = 0; col < 9; col++) {
        int arr[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

        for (int row = 0; row < 9; row++) {
            int num = *((int *)UArray2_at(board, col, row));
            addToArray(arr, num);
            
            if (num > 9 || num < 1) {
                exit(1);
            }
        }
    }
}

/* function checkRows
 * input: the sudoku board with the numbers filled
 * returns: nothing
 * does: goes through board by the rows, and adds the numbers to an array
 * of size 9 initialized with only -1 with the function addToArray to check
 * for repeated numbers
 * error cases: if addToArray detects a repeat, the program exits. if a number
 * on the sudoku board was not between 1 or 9 the program exits.
 */
void checkRows(UArray2_T board)
{
    for (int row = 0; row < 9; row++) {
        int arr[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

        for (int col = 0; col < 9; col++) {
            int num = *((int *)UArray2_at(board, col, row));
            addToArray(arr, num);

            if (num > 9 || num < 1) {
                exit(1);
            }
        }
    }
}

/* function checkBlocks
 * input: the sudoku board with the numbers filled
 * returns: nothing
 * does: calls the function that checks a single 3x3 block on the board on all
 * the 9 blocks of a normal sudoku board.
 */
void checkBlocks(UArray2_T board)
{
    check3by3(board, 0, 3, 0, 3);
    check3by3(board, 0, 3, 3, 6);
    check3by3(board, 0, 3, 6, 9);
    check3by3(board, 3, 6, 0, 3);
    check3by3(board, 3, 6, 3, 6);
    check3by3(board, 3, 6, 6, 9);
    check3by3(board, 6, 9, 0, 3);
    check3by3(board, 6, 9, 3, 6);
    check3by3(board, 6, 9, 6, 9);
}

/* function check3by3
 * input: the sudoku board with the numbers filled, and the bounds of the
 * blocks that make up the 9x9 board.
 * returns: nothing
 * does: goes through a chunk of the board, and adds the numbers to an array
 * of size 9 initialized with only -1 with the function addToArray to check
 * for repeated numbers
 * error cases: if addToArray detects a repeat, the program exits. if a number
 * on the sudoku board was not between 1 or 9 the program exits.
 */
void check3by3(UArray2_T board, int colStartNum, int colEndNum,
                int rowStartNum, int rowEndNum)
{
    int arr[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    for (int col = colStartNum; col < colEndNum; col++) {
        for (int row = rowStartNum; row < rowEndNum; row++) {
            int num = *((int *)UArray2_at(board, col, row));
            addToArray(arr, num);
            if (num > 9 || num < 1) {
                exit(1);
            }
        }
    }
}

/* function addToArray
 * input: an array and the number you want to add to it
 * returns: nothing
 * does: adds the entry on the same index that the number represents. if it
 * tries to add to a slot that was already filled, the program exits.
 */
void addToArray(int values[], int entry)
{
    if (values[entry - 1] == entry) {
        exit(1);
    }
    values[entry - 1] = entry;
}