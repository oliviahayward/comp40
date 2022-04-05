#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

FILE *openFile(char *filename);
float diff(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods);
int checkWidth(Pnm_ppm image1, Pnm_ppm image2);
int checkHeight(Pnm_ppm image1, Pnm_ppm image2);

int main(int argc, char *argv[])
{
    A2Methods_T methods = uarray2_methods_plain;

    if (argc != 3) {
        fprintf(stderr, "Usage: [filename] [filename]\n");
        exit(1);
    }

    FILE *image1file;
    FILE *image2file;

    if (strcmp(argv[1], "-") == 0 && strcmp(argv[2], "-") == 0) {
        fprintf(stderr, "filename needed\n");
        exit(1);
    } else if (strcmp(argv[1], "-") == 0) {
        image2file = openFile(argv[2]);
        image1file = stdin;
    } else if (strcmp(argv[2], "-") == 0) {
        image1file = openFile(argv[1]);
        image2file = stdin;
    } else {
        image1file = openFile(argv[1]);
        image2file = openFile(argv[2]);
    }

    Pnm_ppm image1 = Pnm_ppmread(image1file, methods);
    Pnm_ppm image2 = Pnm_ppmread(image2file, methods);

    float difference = diff(image1, image2, methods);

    printf("%0.4f\n", difference);

}

float diff(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods)
{
    float difference;
    int width = checkWidth(image1, image2);
    int height = checkHeight(image1, image2);
    int image1Denom = image1->denominator;
    int image2Denom = image2->denominator;


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Pnm_rgb value1 = methods->at(image1->pixels, i, j);
            Pnm_rgb value2 = methods->at(image2->pixels, i, j);
            float red1 = (float)value1->red / (float)image1Denom;
            float red2 = (float)value2->red / (float)image2Denom;
            float green1 = (float)value1->green / (float)image1Denom;
            float green2 = (float)value2->green / (float)image2Denom;
            float blue1 = (float)value1->blue / (float)image1Denom;
            float blue2 = (float)value2->blue / (float)image2Denom;
            difference = difference + ((red1 - red2) * (red1 - red2)) +
                         ((green1 - green2) * (green1 - green2)) +
                         ((blue1 - blue2) * (blue1 - blue2));
        }
    }
    difference = difference / (float)(3 * width * height);
    difference = sqrt(difference);
    return difference;
}

int checkWidth(Pnm_ppm image1, Pnm_ppm image2)
{
    int width;
    if (image1->width == image2->width) {
        width = image1->width;
    } else if (image1->width > image2->width) {
        if (image1->width - image2->width > 1) {
            fprintf(stderr, "Image widths differ by more than 1\n");
            printf("1.0\n");
            exit(1);
        } else {
            width = image2->width;
        }
    } else if (image2->width > image1->width) {
        if (image2->width - image1->width > 1) {
            fprintf(stderr, "Image widths differ by more than 1\n");
            printf("1.0\n");
            exit(1);
        } else {
            width = image1->width;
        }
    }
    return width;
}

int checkHeight(Pnm_ppm image1, Pnm_ppm image2)
{
    int height;
    if (image1->height == image2->height) {
        height = image1->height;
    } else if (image1->height > image2->height) {
        if (image1->height - image2->height > 1) {
            fprintf(stderr, "Image heights differ by more than 1\n");
            printf("1.0\n");
            exit(1);
        } else {
            height = image2->height;
        }
    } else if (image2->height > image1->height) {
        if (image2->height - image1->height > 1) {
            fprintf(stderr, "Image heights differ by more than 1\n");
            printf("1.0\n");
            exit(1);
        } else {
            height = image1->height;
        }
    }
    return height;
}

FILE *openFile(char *filename)
{
    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "'%s' is not a valid filename.\n", filename);
        exit(EXIT_FAILURE);
    }

    return file;
}