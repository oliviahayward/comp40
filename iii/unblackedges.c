/* Olivia Hayward
 * Claudia Aranda Barrios
 * unblackedges.c
 * 10/2/2020
 * This file holds the main function which opens the file and runs the program.
 */

#include "blacktowhite.h"

int main(int argc, char *argv[]){

    FILE *pgm = openFile(argc, argv);
    assert(pgm != NULL);

    Bit2_T blackEdges = pgmToBit(pgm);
    Bit2_T newArray = identifyBlackEdges(blackEdges);

    printNewImage(newArray);

    Bit2_free(&newArray);
    fclose(pgm);
    
    return 0;
}