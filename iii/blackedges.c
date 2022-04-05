/* Olivia Hayward
 * Claudia Aranda Barrios
 * blacktowhite.c
 * 10/2/2020
 * This file has the implementation of blacktowhite module that employs
 * breadth-first search to turn the black edge pixels into white.
 */

#include "blacktowhite.h"
#include "assert.h"

Bit2_T pgmToBit(FILE* filename)
{
    Pnmrdr_T reader = Pnmrdr_new(filename);
    Pnmrdr_mapdata mapdata = Pnmrdr_data(reader);

    assert(mapdata.width != 0 && mapdata.height != 0);

    Bit2_T blackEdges = populateArray(reader, mapdata);
    Pnmrdr_free(&reader);

    return blackEdges;
}

Bit2_T populateArray(Pnmrdr_T reader, Pnmrdr_mapdata mapdata)
{
    int width = mapdata.width;
    int height = mapdata.height;
    int type = mapdata.type;

    assert(type == Pnmrdr_bit);

    Bit2_T blackEdges = Bit2_new(width, height);

    int bitValue;
    int prevBit;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            bitValue = Pnmrdr_get(reader);
            prevBit = Bit2_put(blackEdges, col, row, bitValue);
        }
    }

    assert(prevBit == 1 || prevBit == 0);

    return blackEdges;
}


FILE *openFile(int argc, char *argv[])
{
    FILE *file;
    assert(argc < 3);

    if (argc == 2) {
        assert(argv[1] != NULL);
        char *filename = argv[1];
        file = fopen(filename, "r");
        assert(file != NULL);
    } else {
        file = stdin;
        assert(file != NULL);
    }

   return file;
}

Bit2_T identifyBlackEdges(Bit2_T bitMap)
{
   int width = Bit2_width(bitMap);
   int height = Bit2_height(bitMap);

   Seq_T queue = Seq_new(width*height);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (col == 0 || col == width - 1 || row == 0 || row == height - 1) {
                if (Bit2_get(bitMap, col, row) == 1 ) {
                    struct Coords *coord = initializeCoord(col, row);
                    int prev = Bit2_put(bitMap, col, row, 0);
                    queue = addCoordToQueue(queue, coord);

                    assert(prev == 1);

                    while(Seq_length(queue) != 0) {
                        struct Coords *temp = Seq_remlo(queue);
                        int curr_col = temp->col;
                        int curr_row = temp->row;
                        free(temp);

                        queue = checkNeighbors(bitMap, curr_col, curr_row, queue);
                    }
                }
            }
        }
    }

    Seq_free(&queue);

    return bitMap;
}


Seq_T addCoordToQueue(Seq_T queue, struct Coords *coord)
{
   struct Coords *added_coord;
   added_coord = Seq_addhi(queue, coord);

   assert(added_coord != NULL);

   return queue;
}

struct Coords *initializeCoord(int col, int row)
{
    struct Coords *current_bit = malloc(sizeof(struct Coords));
    current_bit->col = col;
    current_bit->row = row;

    return current_bit;
}

Seq_T checkNeighbors(Bit2_T bitMap, int col, int row, Seq_T queue)
{
    int width = Bit2_width(bitMap);
    int height = Bit2_height(bitMap);

    if (col - 1 > -1 && Bit2_get(bitMap, col - 1, row) == 1) {
        blackToWhite(bitMap, col - 1, row);
        struct Coords *toAdd = initializeCoord(col - 1, row);
        queue = addCoordToQueue(queue, toAdd);
    }

    if (col + 1 < width && Bit2_get(bitMap, col + 1, row) == 1) {
        blackToWhite(bitMap, col + 1, row);
        struct Coords *toAdd = initializeCoord(col + 1, row);
        queue = addCoordToQueue(queue, toAdd);
    }


    if (row - 1 > -1 && Bit2_get(bitMap, col, row - 1) == 1) {
        blackToWhite(bitMap, col, row - 1);
        struct Coords *toAdd = initializeCoord(col, row - 1);
        queue = addCoordToQueue(queue, toAdd);
    }

    if (row + 1 < height && Bit2_get(bitMap, col, row+1) == 1) {
        blackToWhite(bitMap, col, row+1);
        struct Coords *toAdd = initializeCoord(col, row + 1);
        queue = addCoordToQueue(queue, toAdd);
    }

    return queue;
}


void blackToWhite(Bit2_T bitMap, int col, int row)
{
    int prev = Bit2_put(bitMap, col, row, 0);
    assert(prev != 0);
}

void printNewImage(Bit2_T bitMap){
    int width = Bit2_width(bitMap);
    int height = Bit2_height(bitMap);

    printf("P1\n");
    printf("%d %d\n", width, height);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            printf("%d ", Bit2_get(bitMap, col, row));
        }
        printf("\n");
    }
}
