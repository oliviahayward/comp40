/* Olivia Hayward
 * Claudia Aranda Barrios
 * HW2
 * sudoku.c
 * 9/28/2020
 *
 * This is the c file calling the functions that check whether a sudoku
 * solution is right or not.
 */

#include "checker.h"

int main(int argc, char *argv[]){

    FILE *pgm = openFile(argc, argv);

    UArray2_T sudokuBoard = pgmToArray(pgm);
    checkSolution(sudokuBoard);

    UArray2_free(&sudokuBoard);
    fclose(pgm);

    return 0;
}