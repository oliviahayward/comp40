/* Olivia Hayward and Meghan Kloud
 * ohaywa02 and mkloud01
 * HW4 arith
 * 40image.c
 * 10/26/2020
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "pnm.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "a2methods.h"
#include "seq.h"
#include "arith40.h"
#include "math.h"
#include "compress.h"
#include "decompress.h"
#include "imgManipulations.h"

Pnm_ppm handleDecompression(FILE *fp);
Seq_T handleCompression(Pnm_ppm croppedImage, A2Methods_mapfun map);
bool compOrDecomp(int argc, char *argv[], int *i);
FILE *openFile(int argc, char *argv[], int i);
void writeCompressed(unsigned width, unsigned height, Seq_T compressedVals);
Seq_T readWords(FILE *fp, unsigned width, unsigned height);

int main(int argc, char *argv[])
{
    int i;
    bool compressOrDecompress = compOrDecomp(argc, argv, &i);

    assert(argc - i <= 1);    /* at most one file on command line */
    FILE *fp = openFile(argc, argv, i);
    assert(fp != NULL);

    A2Methods_T methods = uarray2_methods_blocked;

    /* COMPRESSION */
    if (compressOrDecompress == true) {
        A2Methods_mapfun *map = methods->map_block_major;
        Pnm_ppm original = Pnm_ppmread(fp, methods);
        Pnm_ppm croppedImage = crop(original, map);
        Seq_T compressedVals = handleCompression(croppedImage, map);

        writeCompressed(croppedImage->width, croppedImage->height,
                        compressedVals);

        Pnm_ppmfree(&croppedImage);

        freeCompressed(compressedVals);
    }

    /* DECOMPRESSION */
    else if (compressOrDecompress == false) {
        Pnm_ppm newImage = handleDecompression(fp);

        Pnm_ppmwrite(stdout, newImage);
        Pnm_ppmfree(&newImage);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

/* Function: compOrDecomp
 * Input: Command line arguments, array of arguments, and an integer that
 *        keeps track of its place in the command line
 * Does: Determines whether to compress or decompress based on the command line
 *       argument given. Keeps track of the number of command line arguments
 *       read.
 * Returns: Boolean that is true if it's compression and false if it's
 *          decompression.
 * Error checks: Checks that the commands line arguments are correct.
 */ 
bool compOrDecomp(int argc, char *argv[], int *i)
{
    bool compressOrDecompress;
    for (int j = 1; j < argc; j++) {
        if (strcmp(argv[j], "-c") == 0) {
            compressOrDecompress = true;
        } else if (strcmp(argv[j], "-d") == 0) {
            compressOrDecompress = false;
        } else if (*argv[j] == '-') {
            fprintf(stderr, "%s: unknown option '%s'\n",
                            argv[0], argv[j]);
            exit(1);
        } else if (argc - j > 2) {
            fprintf(stderr, "Usage: %s -d [filename]\n"
                            "       %s -c [filename]\n",
                                    argv[0], argv[0]);
            exit(1);
        } else {
            *i = j;
            break;
        }
    }
    return compressOrDecompress;
}

/* Function: openFile
 * Input: Command line arguments, array of arguments, and an integer that
 *        keeps track of its place in the command line
 * Does: Checks to see if a file is given on the command line and opens it if
 *       so. If there is no file given, it reads from standard input.
 * Returns: A file pointer
 * Error checks: Checks that the file is open and that it exists. If it fails,
 *               it is a CRE.
 */
FILE *openFile(int argc, char *argv[], int i)
{
    FILE *fp;
    if (i < argc) {
        fp = fopen(argv[i], "r");
        assert(fp != NULL);

    } else {
        fp = stdin;
        assert(fp != NULL);
    }

    return fp;
}

/* Function: writeCompressed
 * Input: width, height, a sequence of compressed values
 * Does: Writes the compressed image file to standard output
 * Returns: Nothing
 * Error checks: Checks that the sequence isn't NULL.
 */
void writeCompressed(unsigned width, unsigned height, Seq_T compressedVals)
{
    assert(compressedVals != NULL);

    printf("COMP40 Compressed image format 2\n%u %u", width, height);
    printf("\n");
    
    for (int i = 0; i < ((int)(height / 2) * (int)(width / 2)); i++) {
        uint64_t *packedValue = Seq_get(compressedVals, i);

        uint64_t wordByte = Bitpack_getu(*packedValue, 8, 24);
        putchar(wordByte);
        wordByte = Bitpack_getu(*packedValue, 8, 16);
        putchar(wordByte);
        wordByte = Bitpack_getu(*packedValue, 8, 8);
        putchar(wordByte);
        wordByte = Bitpack_getu(*packedValue, 8, 0);
        putchar(wordByte);
    }
}

/* Function: handleCompression
 * Input: a cropped PPM and a map function
 * Does: Calls the compression functions and sets the new compressed codewords
 *       into a sequence newValues
 * Returns: a sequence of compressed codewords
 * Error checks: If the cropped PPM, the map function, or the codeword are
 *               NULL, it's a CRE.
 */
Seq_T handleCompression(Pnm_ppm croppedImage, A2Methods_mapfun map)
{
    assert(croppedImage != NULL && map != NULL);
    int blocksize = croppedImage->methods->blocksize(croppedImage->pixels);

    struct Pixels final;
    final.blocks = Seq_new(blocksize * blocksize);
    final.blocksize = blocksize;
    final.denominator = croppedImage->denominator;

    (* map)(croppedImage->pixels, convertBlocked, &final);

    Seq_T newValues = Seq_new(Seq_length(final.blocks));

    for (int i = 0; i < Seq_length(final.blocks); i++) {
        uint64_t *result = malloc(sizeof(uint64_t));
        assert(result != NULL);

        *(result) = compress(Seq_get(final.blocks, i), final.denominator);
        Seq_addhi(newValues, result);
    }

    free2DSequence(final.blocks);

    return newValues;
}

/* Function: handleDecompression
 * Input: File pointer to a compressed binary image file
 * Does: Calls all the decompression functions and creates a new PPM image to
 *       store the decompressed image
 * Returns: a Pnm_ppm image pointer
 * Error checks: If the file pointer or the new PPM is NULL, it's a CRE. If
 *               the width, height, denominator, or read-in 'block' is less
 *               than or equal to 0, it is a CRE.
 */
Pnm_ppm handleDecompression(FILE *fp)
{
    assert(fp != NULL);
    unsigned height, width;
    int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", &width,
                                                                    &height);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');
    assert(width > 0 && height > 0);

    A2Methods_T methods = uarray2_methods_blocked;
    A2Methods_mapfun *map = methods->map_block_major;

    A2Methods_UArray2 newArray = methods->new_with_blocksize(width, height,
                                                 sizeof(struct Pnm_rgb), 2);
    int denominator = 255;

    struct Pnm_ppm *newImage = malloc(sizeof(struct Pnm_ppm));
    assert(newImage != NULL);
    assert(denominator > 0);

    newImage->width = width;
    newImage->height = height;
    newImage->denominator = denominator;
    newImage->pixels = newArray;
    newImage->methods = methods;

    Seq_T newValues = readWords(fp, width, height);
    (* map)(newArray, convertSequence, newValues);
    newImage->pixels = newArray;

    free2DSequence(newValues);

    return newImage;
}

/* Function: readWords
 * Input: a file pointer, width of cropped image, and height of cropped image
 * Does: Reads in the bits from the file and puts them into a bitpacked word to
 *       be decompressed.
 * Returns: A sequence of 32-bit codewords
 * Error checks: Checks that the file pointer isn't NULL. Checks that the
 *               correct amount of codewords are being read in from the file.
 *               Checks that the size of the codewords are 32 bits.
 */
Seq_T readWords(FILE *fp, unsigned width, unsigned height)
{
    assert(fp != NULL);

    Seq_T newValues = Seq_new((width * height) / 4);

    uint64_t block = 0;
    for (int i = 0; i < (((int)height / 2) * ((int)width) / 2); i++){
        uint64_t word = 0;
        int j = 32;
        while (j > 0 ) {
            j = j - 8;
            block = fgetc(fp);
            assert((int)block != EOF);
            word = Bitpack_newu(word, 8, j, block);
        }
        //assert(sizeof(word) = 4);
        Seq_T newBlock = decompress(word);
        Seq_addhi(newValues, newBlock);
    }

    return newValues;
}