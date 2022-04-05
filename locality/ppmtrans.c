/******************************************************************************
 *
 * Filename: ppmtrans.c
 * By: Meghan Kloud (mkloud01) and Olivia Hayward (ohaywa02)
 * Date: 10/12/2020
 * Assignment: Homework 3 Part C: ppmtrans
 * Summary: This file contains the implementation for our ppmtrans file and 
 * functions. Read function contracts for more details.
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

FILE *openFile(char *filename);
void rotate(Pnm_ppm image, A2Methods_T methods, A2Methods_mapfun *map,
                  int rotation, bool timeCheck, char *time_file_name);
double rotate90(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                                A2Methods_mapfun *map);
void rotate90Apply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                            void *cl);
double rotate180(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                                 A2Methods_mapfun *map);
void rotate180Apply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                             void *cl);
void flipFun(Pnm_ppm image, A2Methods_T methods, A2Methods_mapfun *map,
                     char *flip, bool timeCheck, char *time_file_name);
double flipHoriz(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                                 A2Methods_mapfun *map);
void flipHorizApply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                            void *cl);
double flipVert(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                                A2Methods_mapfun *map);
void flipVertApply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                            void *cl);
void printPix(A2Methods_UArray2 array2, int width, int height,
                                         A2Methods_T methods);
void timingFun(Pnm_ppm image, char *time_file_name, double time_used);

#define SET_METHODS(METHODS, MAP, WHAT) do {                \
    methods = (METHODS);                                    \
    assert(methods != NULL);                                \
    map = methods->MAP;                                     \
    if (map == NULL) {                                      \
        fprintf(stderr, "%s does not support "              \
                        WHAT "mapping\n",                   \
                        argv[0]);                           \
        exit(1);                                            \
    }                                                       \
} while (0)

static void
usage(const char *progname)
{
    fprintf(stderr, "Usage: %s [-rotate <angle>] "
                    "[-{row,col,block}-major] [filename]\n",
                    progname);
    exit(1);
}

int main(int argc, char *argv[])
{
    char *time_file_name = NULL;
    char *flip           = NULL;
    int   rotation       = 0;
    int   i;
    bool flipCheck       = false;
    bool transposeCheck  = false;
    bool checkForFile    = false;
    bool timeCheck       = false;
    bool rotationCheck   = false;

    /* default to UArray2 methods */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);

    /* default to best map */
    A2Methods_mapfun *map = methods->map_default;
    assert(map);

    Pnm_ppm image;
    FILE * ppm;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-row-major") == 0) {
            SET_METHODS(uarray2_methods_plain, map_row_major,
                                                 "row-major");

        } else if (strcmp(argv[i], "-col-major") == 0) {
            SET_METHODS(uarray2_methods_plain, map_col_major,
                                              "column-major");

        } else if (strcmp(argv[i], "-block-major") == 0) {
            SET_METHODS(uarray2_methods_blocked, map_block_major,
                                                   "block-major");

        } else if (strcmp(argv[i], "-rotate") == 0) {
            if (!(i + 1 < argc)) {      /* no rotate value */
                usage(argv[0]);
            }
            if (flipCheck){
                rotationCheck = false;
            } else {
                rotationCheck = true;
            }

            char *endptr;
            rotation = strtol(argv[++i], &endptr, 10);

            if (!(rotation == 0 || rotation == 90 ||
                rotation == 180 || rotation == 270)) {
                fprintf(stderr,
                        "Rotation must be 0, 90 180 or 270\n");
                        usage(argv[0]);
                }
            
            if (!(*endptr == '\0')) {    /* Not a number */
                usage(argv[0]);
            }

        } else if (strcmp(argv[i], "-flip") == 0) {
            if (!(i + 1 < argc)) {      /* no flip value */
                usage(argv[0]);
            }
            if (rotationCheck){
                flipCheck = false;
            } else {
                flipCheck = true; /* flip has been specified */
            }

            flip = argv[++i];

            if (!((strcmp(flip, "vertical") == 0 || (strcmp(flip, "horizontal"))
                                                                       == 0))) {
                fprintf(stderr, "Flip must be horizontal or vertical\n");
                        usage(argv[0]);
            }
        } else if (strcmp(argv[i], "-transpose") == 0) {
            transposeCheck = true;
        } else if (strcmp(argv[i], "-time") == 0) {
            time_file_name = argv[++i];
            /* Tells the program that it should record the time it takes to do
             * the rotation */
            timeCheck = true;

        } else if (*argv[i] == '-') {
            fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                argv[i]);
        } else if (argc - i > 1) {
            fprintf(stderr, "Too many arguments\n");
            usage(argv[0]);
        } else {
            if (i == (argc - 1)) {
                /* Tells the program it should read from a file, not stdin */
                checkForFile = true;
            }
            break;
        }
    }

    if (checkForFile == false) {
        ppm = stdin;
        image = Pnm_ppmread(ppm, methods);
    } else {
        ppm = openFile(argv[argc - 1]);
          image = Pnm_ppmread(ppm, methods);
    }

    if (rotationCheck) {
        rotate(image, methods, map, rotation, timeCheck, time_file_name);
    }
    
    if (flipCheck == true) {
        flipFun(image, methods, map, flip, timeCheck, time_file_name);
    }
    
    if (transposeCheck == true) {
        double time_used = flipVert(image, image->pixels, methods, map);
        time_used += flipHoriz(image, image->pixels, methods, map);
        if (timeCheck == true) {
            timingFun(image, time_file_name, time_used);
        }
    }

    Pnm_ppmwrite(stdout, image);

    printPix(image->pixels, image->width, image->height, methods);

    Pnm_ppmfree(&image);
    fclose(ppm);

    return 0;
}

/* Function: openFile
 * Input: Filename string
 * Returns: File pointer
 * Does: Opens the file given and returns a pointer to that file
 * Error checks: If the file pointer is NULL, print to stderr and exit the
 *               program.
 */
FILE *openFile(char *filename)
{
    FILE *file;

    file = fopen(filename, "rw");

    if (file == NULL) {
        fprintf(stderr, "'%s' is not a valid filename.\n", filename);
        exit(EXIT_FAILURE);
    }

    return file;
}

/* Function: rotate
 * Input: a Pnm_ppm struct, A2Methods_T methods, a pointer to an A2Methods
 *        map function, rotation degree, and a timeCheck bool
 * Returns: Nothing
 * Does: Calls the specified rotate function based on the command line argument
 *       (stored in the variable rotation) and calls timingFun if timeCheck is
 *       true.
 * Error checks: If the Pnm_ppm struct, methods, or the pointer to the map
 *               function is NULL, exit the program with a CRE. 
 */
void rotate(Pnm_ppm image, A2Methods_T methods, A2Methods_mapfun *map,
                   int rotation, bool timeCheck, char *time_file_name)
{
    assert(image != NULL && methods != NULL && map != NULL);
    if (rotation == 90) {
        double time_used = rotate90(image, image->pixels, methods, map);
        if (timeCheck == true) {
            timingFun(image, time_file_name, time_used);
        }
    }
    if (rotation == 180) {
        double time_used = rotate180(image, image->pixels, methods, map);
        if (timeCheck == true) {
            timingFun(image, time_file_name, time_used);
        }
    }
    if (rotation == 270) {
        double time_used = rotate180(image, image->pixels, methods, map);
        time_used += rotate90(image, image->pixels, methods, map);
        if (timeCheck == true) {
            timingFun(image, time_file_name, time_used);
        }
    }
}

/* Function: rotate90
 * Input: a Pnm_ppm struct, an A2Methods_T struct, A2Methods_T methods, and
 *        a pointer to an A2Methods map function
 * Returns: the total amount of CPU time it took tp do the rotation
 * Does: Creates a new instance of A2Methods_UArray2 and sets the array equal
 *       to the Pnm_ppm's array. It then sets the Pnm_ppm array to a new empty
 *       array. It begins the timer and calls the map function to do the
 *       rotation. It then frees the timer and the original array.
 * Error checks: If the Pnm_ppm struct, A2Methods_T array2, methods, the Pnm_ppm
 *               array, pointer to the map function, or CPUTime_T timer equals
 *               NULL, exit the program with a CRE. If the time_used is less
 *               than or equal to 0, exit with a CRE. If the width or the height
 *               is less than or equal to 0, exit with a CRE.
 */
double rotate90(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                               A2Methods_mapfun *map)
{
    assert(image != NULL && array2 != NULL && methods != NULL && map != NULL);
    assert(image->pixels != NULL);

    A2Methods_UArray2 original = image->pixels;
    image->pixels = methods->new(image->height, image->width,
                                       methods->size(array2));

    int width = image->width;
    int height = image->height;
    assert(width > 0 && height > 0);

    image->width = height;
    image->height = width;

    CPUTime_T timer = CPUTime_New();
    assert(timer != NULL);

    double time_used;
    CPUTime_Start(timer);

    (* map)(original, rotate90Apply, image);
    time_used = CPUTime_Stop(timer);
    assert(time_used > 0);

    methods->free(&original);
    CPUTime_Free(&timer);

    return time_used;
}

/* Function: rotate90apply
 * Input: column, row, A2Methods_UArray2 array, pointer to the element, pointer
 *        to a closure argument
 * Returns: Nothing
 * Does: Sets the column and row equal to their new rotated coordinates and sets
 *       the values of the red, green, and blue to their new values in the new
 *       array.
 * Error checks: If the column, row, new column, or new row are less than 0,
 *               exit with a CRE. If the A2Methods_UArray2, elem pointer,
 *               closure argument, or Pnm_rgb struct are NULL, exit with a CRE.
 */
void rotate90Apply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                             void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array2 != NULL && elem != NULL && cl != NULL);

    int col = ((Pnm_ppm)cl)->width - 1 - j;
    int row = i;

    assert(col >= 0 && row >= 0);

    Pnm_rgb oldElem = ((Pnm_ppm)cl)->methods->at(((Pnm_ppm)cl)->pixels, col,
                                                                        row);

    assert(oldElem != NULL);

    oldElem->red = ((Pnm_rgb)elem)->red;
    oldElem->green = ((Pnm_rgb)elem)->green;
    oldElem->blue = ((Pnm_rgb)elem)->blue;

    (void) array2;
}

/* Function: rotate180
 * Input: a Pnm_ppm struct, an A2Methods_T struct, A2Methods_T methods, and
 *        a pointer to an A2Methods map function
 * Returns: the total amount of CPU time it took tp do the rotation
 * Does: Creates a new instance of A2Methods_UArray2 and sets the array equal
 *       to the Pnm_ppm's array. It then sets the Pnm_ppm array to a new empty
 *       array. It begins the timer and calls the map function to do the
 *       rotation. It then frees the timer and the original array.
 * Error checks: If the Pnm_ppm struct, A2Methods_T array2, methods, the Pnm_ppm
 *               array, pointer to the map function, or CPUTime_T timer equals
 *               NULL, exit the program with a CRE. If the time_used is less
 *               than or equal to 0, exit with a CRE.
 */
double rotate180(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                                A2Methods_mapfun *map)
{
    assert(image != NULL && array2 != NULL && methods != NULL && map != NULL);
    assert(image->pixels != NULL);

    A2Methods_UArray2 original = image->pixels;
    image->pixels = methods->new(image->width, image->height,
                                      methods->size(array2));

    CPUTime_T timer = CPUTime_New();
    assert(timer != NULL);

    double time_used;
    CPUTime_Start(timer);

    (* map)(original, rotate180Apply, image);
    time_used = CPUTime_Stop(timer);
    assert(time_used > 0);

    methods->free(&original);
    CPUTime_Free(&timer);

    return time_used;
}

/* Function: rotate180apply
 * Input: column, row, A2Methods_UArray2 array, pointer to the element, pointer
 *        to a closure argument
 * Returns: Nothing
 * Does: Sets the column and row equal to their new rotated coordinates and sets
 *       the values of the red, green, and blue to their new values in the new
 *       array.
 * Error checks: If the column, row, new column, or new row are less than 0,
 *               exit with a CRE. If the A2Methods_UArray2, elem pointer,
 *               closure argument, or Pnm_rgb struct are NULL, exit with a CRE.
 */
void rotate180Apply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                              void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array2 != NULL && elem != NULL && cl != NULL);

    int col = ((Pnm_ppm)cl)->width - 1 - i;
    int row = ((Pnm_ppm)cl)->height - 1 - j;

    assert(col >= 0 && row >= 0);

    Pnm_rgb oldElem = ((Pnm_ppm)cl)->methods->at(((Pnm_ppm)cl)->pixels, col,
                                                                        row);

    assert(oldElem != NULL);

    oldElem->red = ((Pnm_rgb)elem)->red;
    oldElem->green = ((Pnm_rgb)elem)->green;
    oldElem->blue = ((Pnm_rgb)elem)->blue;

    (void) array2;
}

/* Function: flip
 * Input: a Pnm_ppm struct, A2Methods_T methods, a pointer to an A2Methods
 *        map function, flip string, and a timeCheck bool
 * Returns: Nothing
 * Does: Calls the specified flip function based on the command line argument
 *       (stored in the variable flip) and calls timingFun if timeCheck is
 *       true.
 * Error checks: If the Pnm_ppm struct, methods, or the pointer to the map
 *               function is NULL, exit the program with a CRE. 
 */
void flipFun(Pnm_ppm image, A2Methods_T methods, A2Methods_mapfun *map,
                      char* flip, bool timeCheck, char *time_file_name)
{
    assert(image != NULL && methods != NULL && map != NULL);
    if (strcmp(flip, "horizontal") == 0 ) {
        double time_used = flipHoriz(image, image->pixels, methods, map);
        if (timeCheck == true) {
           timingFun(image, time_file_name, time_used);
        }
    }
    if (strcmp(flip, "vertical") == 0 ) {
        double time_used = flipVert(image, image->pixels, methods, map);
        if (timeCheck == true) {
            timingFun(image, time_file_name, time_used);
        }
    }
}

/* Function: flipHoriz
 * Input: a Pnm_ppm struct, an A2Methods_T struct, A2Methods_T methods, and
 *        a pointer to an A2Methods map function
 * Returns: the total amount of CPU time it took tp do the rotation
 * Does: Creates a new instance of A2Methods_UArray2 and sets the array equal
 *       to the Pnm_ppm's array. It then sets the Pnm_ppm array to a new empty
 *       array. It begins the timer and calls the map function to do the
 *       rotation. It then frees the timer and the original array.
 * Error checks: If the Pnm_ppm struct, A2Methods_T array2, methods, the Pnm_ppm
 *               array, pointer to the map function, or CPUTime_T timer equals
 *               NULL, exit the program with a CRE. If the time_used is less
 *               than or equal to 0, exit with a CRE.
 */ 
double flipHoriz(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                                A2Methods_mapfun *map)
{
    assert(image != NULL && array2 != NULL && methods != NULL && map != NULL);
    assert(image->pixels != NULL);

    A2Methods_UArray2 original = image->pixels;
    image->pixels = methods->new(image->width, image->height,
                                      methods->size(array2));

    CPUTime_T timer = CPUTime_New();
    assert(timer != NULL);

    double time_used;
    CPUTime_Start(timer);

    (* map)(original, flipHorizApply, image);
    time_used = CPUTime_Stop(timer);
    assert(time_used > 0);

    methods->free(&original);
    CPUTime_Free(&timer);

    return time_used;
}

/* Function: flipHorizApply
 * Input: column, row, A2Methods_UArray2 array, pointer to the element, pointer
 *        to a closure argument
 * Returns: Nothing
 * Does: Sets the column and row equal to their new flipped coordinates and sets
 *       the values of the red, green, and blue to their new values in the new
 *       array.
 * Error checks: If the column, row, new column, or new row are less than 0,
 *               exit with a CRE. If the A2Methods_UArray2, elem pointer,
 *               closure argument, or Pnm_rgb struct are NULL, exit with a CRE.
 */

void flipHorizApply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                              void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array2 != NULL && elem != NULL && cl != NULL);

    int col = ((Pnm_ppm)cl)->width - i - 1;
    int row = j;

    assert(col >= 0 && row >= 0);

    Pnm_rgb oldElem = ((Pnm_ppm)cl)->methods->at(((Pnm_ppm)cl)->pixels, col,
                                                                        row);

    assert(oldElem != NULL);

    oldElem->red = ((Pnm_rgb)elem)->red;
    oldElem->green = ((Pnm_rgb)elem)->green;
    oldElem->blue = ((Pnm_rgb)elem)->blue;

    (void) array2;
}

/* Function: flipVert
 * Input: a Pnm_ppm struct, an A2Methods_T struct, A2Methods_T methods, and
 *        a pointer to an A2Methods map function
 * Returns: the total amount of CPU time it took tp do the rotation
 * Does: Creates a new instance of A2Methods_UArray2 and sets the array equal
 *       to the Pnm_ppm's array. It then sets the Pnm_ppm array to a new empty
 *       array. It begins the timer and calls the map function to do the
 *       rotation. It then frees the timer and the original array.
 * Error checks: If the Pnm_ppm struct, A2Methods_T array2, methods, the Pnm_ppm
 *               array, pointer to the map function, or CPUTime_T timer equals
 *               NULL, exit the program with a CRE. If the time_used is less
 *               than or equal to 0, exit with a CRE.
 */ 
double flipVert(Pnm_ppm image, A2Methods_T array2, A2Methods_T methods,
                                                A2Methods_mapfun *map)
{
    assert(image != NULL && array2 != NULL && methods != NULL && map != NULL);
    assert(image->pixels != NULL);

    A2Methods_UArray2 original = image->pixels;
    image->pixels = methods->new(image->width, image->height,
                                      methods->size(array2));

    CPUTime_T timer = CPUTime_New();
    assert(timer != NULL);

    double time_used;
    CPUTime_Start(timer);

    (* map)(original, flipVertApply, image);
    time_used = CPUTime_Stop(timer);
    assert(time_used > 0);

    methods->free(&original);
    CPUTime_Free(&timer);

    return time_used;
}

/* Function: flipVertApply
 * Input: column, row, A2Methods_UArray2 array, pointer to the element, pointer
 *        to a closure argument
 * Returns: Nothing
 * Does: Sets the column and row equal to their new flipped coordinates and sets
 *       the values of the red, green, and blue to their new values in the new
 *       array.
 * Error checks: If the column, row, new column, or new row are less than 0,
 *               exit with a CRE. If the A2Methods_UArray2, elem pointer,
 *               closure argument, or Pnm_rgb struct are NULL, exit with a CRE.
 */
void flipVertApply(int i, int j, A2Methods_UArray2 array2, void *elem,
                                                              void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array2 != NULL && elem != NULL && cl != NULL);

    int col = i;
    int row = ((Pnm_ppm)cl)->height - j - 1;

    assert(col >= 0 && row >= 0);

    Pnm_rgb oldElem = ((Pnm_ppm)cl)->methods->at(((Pnm_ppm)cl)->pixels, col,
                                                                        row);

    assert(oldElem != NULL);

    oldElem->red = ((Pnm_rgb)elem)->red;
    oldElem->green = ((Pnm_rgb)elem)->green;
    oldElem->blue = ((Pnm_rgb)elem)->blue;

    (void) array2;
}

/* Function: printPix
 * Input: A2Methods_UArray2, width, height, A2Methods_T methods
 * Returns: Nothing
 * Does: Iterates through the array and prints out the red, green, and blue
 *       values of each pixel
 * Error checks: If uarray2, methods, or the at function return NULL, then exit
 *               with a CRE. If the width or the height is less than or equal to
 *               0, exit with a CRE.
 */
void printPix(A2Methods_UArray2 array2, int width, int height,
                                          A2Methods_T methods)
{
    assert(array2 != NULL && methods != NULL);
    assert(width > 0 && height > 0);

    Pnm_rgb toPrint;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            toPrint = methods->at(array2, x, y);
            assert(toPrint != NULL);
            
            if (x == 0) {
                printf("%u %u %u", toPrint->red, toPrint->green,
                                                 toPrint->blue);
            }
            else
            {
                printf("   %u %u %u", toPrint->red, toPrint->green,
                                                    toPrint->blue);
            }
        }
        printf("\n");
    }
}

/* Function: timingFun
 * Input: Pnm_ppm struct, filename string, time used double
 * Returns: Nothing
 * Does: Opens the timing file and calculates the total time per pixel based on
 *       the time_used variable. Then it writes that information to the
 *       specified file.
 * Error checks: If the Pnm_ppm, filename string, or file pointer is NULL, exit
 *               with a CRE. If the totalPixels or the time_used are less than
 *               or equal to 0, exit with a CRE.
 */
void timingFun(Pnm_ppm image, char *time_file_name, double time_used){
    assert(image != NULL && time_file_name != NULL);
    FILE *timings_file = fopen(time_file_name, "a");

    assert(timings_file != NULL);

    int totalPixels = (image->width * image->height);
    assert(totalPixels > 0 && time_used > 0);

    double timePerPixel = time_used / totalPixels;

    fprintf(timings_file, "Image height: %d   Image width: %d\nTotal time: %lf"
         " nanoseconds\nTime per pixel: %lf nanoseconds\n", (int) image->width,
                                (int) image->height, time_used, timePerPixel);
}