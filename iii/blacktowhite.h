/* Olivia Hayward
 * Claudia Aranda Barrios
 * blacktowhite.h
 * 10/2/2020
 * This header file declares all of the functions that will be used in
 * blacktowhite.c
 */

#ifndef _BLACKTOWHITE_H_
#define _BLACKTOWHITE_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "assert.h"
#include "pnmrdr.h"
#include "bit2.h"
#include "seq.h"

struct Coords
{
    int row;
    int col;
};

Bit2_T pgmToBit(FILE* filename);

FILE *openFile(int argc, char *argv[]);

void printNewImage(Bit2_T bitMap);

Bit2_T populateArray(Pnmrdr_T reader, Pnmrdr_mapdata mapdata);

Bit2_T identifyBlackEdges(Bit2_T bitMap);

Seq_T addCoordToQueue(Seq_T stack, struct Coords *coord);

struct Coords *initializeCoord(int col, int row);

Seq_T checkNeighbors(Bit2_T bitMap, int col, int row, Seq_T queue);

void blackToWhite(Bit2_T bitMap, int col, int row);

#endif