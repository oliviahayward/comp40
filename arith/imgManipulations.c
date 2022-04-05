/* Olivia Hayward and Meghan Kloud
 * ohaywa02 and mkloud01
 * HW4 arith
 * imgManipulations.c
 * 10/26/2020
 */

#include "imgManipulations.h"

/* Function: crop
 * Input: a PPM image, pointer to an A2Methods map function
 * Does: If the image has an uneven number of rows or columns, it will crop
 *       the image so that it has an even number.
 * Returns: A pointer to the new cropped image
 * Error checks: If the PPM image or the map fuction are NULL, it is a CRE.
 */
Pnm_ppm crop(Pnm_ppm image, A2Methods_mapfun *map)
{
    assert(image != NULL && map != NULL);
    int width = image->width;
    int height = image->height;
    int blocksize = 2;

    A2Methods_UArray2 original = image->pixels;

    if (width % blocksize != 0) {
        width = width - 1;
    }
    if (height % blocksize != 0) {
        height = height - 1;
    }

    image->pixels = image->methods->new_with_blocksize(width, height,
                                    image->methods->size(image->pixels),
                                    blocksize);
    image->width = width;
    image->height = height;

    (* map)(original, populate, image);

    image->methods->free(&original);

    return image;
}

/* Function: populate
 * Input: The coordinates of the element, an array, a void pointer to an
 *        element, and a void pointer to the closure argument
 * Does: It iterates through the closure argument (a PPM image file) and
 *       sets the PPM image file's Pnm_rgb struct to the Pnm_rgb struct of the
 *       element being passed into the function
 * Returns: Nothing
 * Error checks: If i or j is less than 0, it is a CRE. If array2, elem, or the
 *               closure argument are NULL, it is a CRE. If the element at the
 *               closure argument is NULL, it is a CRE.
 */
void populate(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array2 != NULL && elem != NULL && cl != NULL);

    if (i < ((int)((Pnm_ppm)cl)->width) && j < ((int)((Pnm_ppm)cl)->height)) {
        Pnm_rgb oldElem = ((Pnm_ppm)cl)->methods->at(((Pnm_ppm)cl)->pixels,
                                                       i, j);

        assert(oldElem != NULL);

        oldElem->red = ((Pnm_rgb)elem)->red;
        oldElem->green = ((Pnm_rgb)elem)->green;
        oldElem->blue = ((Pnm_rgb)elem)->blue;
    }

    (void) array2;
}

/* Function: convertSequence
 * Input: the coordinates of the element, an array, a void pointer to an
 *        element, and a void pointer to a closure argument
 * Does: It takes a sequence of sequences and sets the element's Pnm_rgb struct
 *       equal to the Pnm_rgb values of an element in currentSeq
 * Returns: Nothing
 * Error checks: If i or j is less than 0, it is a CRE. If array2, elem, or the
 *               closure argument are NULL, it is a CRE. If the element at the
 *               closure argument is NULL, it is a CRE.
 */
void convertSequence(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array2 != NULL && elem != NULL && cl != NULL);
    Seq_T seq = (Seq_T)cl;

    for (int k = 0; k < Seq_length(seq); k++) {
        Seq_T currentSeq = Seq_get(seq, k);
        
        if (Seq_length(currentSeq) == 0) {
            continue;
        } else {
            Pnm_rgb temp = Seq_get(currentSeq, 0);
            ((Pnm_rgb)elem)->red = temp->red;
            ((Pnm_rgb)elem)->green = temp->green;
            ((Pnm_rgb)elem)->blue = temp->blue;
            free(temp);
            Seq_remlo(currentSeq);
            break;
        }
    }

    (void) i;
    (void) j;
    (void) array2;
}

/* Function: convertBlocked
 * Input: the coordinates of the element, an array, a void pointer to an
 *        element, and a void pointer to a closure argument
 * Does: Converts a 2D array into a sequence of blocks
 * Returns: Nothing
 * Error checks: If i or j is less than 0, it is a CRE. If array2, elem, or the
 *               closure argument are NULL, it is a CRE. If the element at the
 *               closure argument is NULL, it is a CRE. If the blocksize of the
 *               Pixel struct is equal to or less than 0, it is a CRE.
 */
void convertBlocked(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array2 != NULL && elem != NULL && cl != NULL);
    (void) i;
    (void) j;
    (void) array2;

    Seq_T currentSeq = ((struct Pixels *)cl)->blocks;

    assert(((struct Pixels *)cl)->blocksize > 0);

    int blockSize = ((struct Pixels *)cl)->blocksize *
                    ((struct Pixels *)cl)->blocksize;

    if ((Seq_length(currentSeq) == 0) || (Seq_length(Seq_get(currentSeq, Seq_length(currentSeq) - 1)) == 4)) {
        Seq_T block = Seq_new(blockSize);
        Seq_addhi(block, elem);
        Seq_addhi(currentSeq, block);
    } else {
        Seq_addhi(Seq_get(currentSeq, Seq_length(currentSeq) - 1), elem);
    }
}

/* Function: freeCompressed
 * Input: A sequence of compressed values
 * Does: Frees all the codewords and then the sequence
 * Returns: Nothing
 * Error checks: Checks that compressedVals and temp aren't NULL.
 */
void freeCompressed(Seq_T compressedVals)
{
    assert(compressedVals != NULL);
    for (int j = 0; j < Seq_length(compressedVals); j++) {
        uint64_t *temp = Seq_get(compressedVals, j);
        assert(temp != NULL);
        free(temp);
    }

    Seq_free(&compressedVals);
}

/* Function: free2DSequence
 * Input: A sequence of sequences
 * Does: Frees all the nested sequences and then the larger sequence
 * Returns: Nothing
 * Error checks: Checks that newValues and temp aren't NULL.
 */
void free2DSequence(Seq_T newValues)
{
    assert(newValues != NULL);
    for (int l = 0; l < Seq_length(newValues); l++) {
        Seq_T temp = Seq_get(newValues, l);
        assert(temp != NULL);
        Seq_free(&temp);
    }

    Seq_free(&newValues);
}