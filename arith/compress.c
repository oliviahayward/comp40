/* Olivia Hayward and Meghan Kloud
 * ohaywa02 and mkloud01
 * HW4 arith
 * compress.c
 * 10/26/2020
 */

#include "compress.h"

/* Holds the Y/Pb/Pr values for a pixel */
struct YPbPr {
    float y;
    float Pb;
    float Pr;
};

/* Holds the a/b/c/d/average Pb/average Pr values for a block */
struct FinalVals {
    int a;
    int b;
    int c;
    int d;
    unsigned avgPb;
    unsigned avgPr;
};

struct YPbPr rgbToYpp(Pnm_rgb pixel, int denominator);
struct FinalVals calcFinalVals(Seq_T YPbPrVals);
struct FinalVals convertFinalVals(float a, float b, float c, float d, float avgPb, float avgPr);
uint64_t bitpack(struct FinalVals final);
void checkVal(float *x, float upper, float lower);

/* Function: compress
 * Does: Acts as the interface for compression. Calls all of the compression
 *       steps in order. See helper function contracts for more details.
 * Input: A sequence of blocks and a denominator
 * Returns: a 32-bit codeword for the compressed block
 * Error checks: If block is NULL or denominator is less than 1, it is a CRE.
 */
uint64_t compress(Seq_T block, int denominator)
{
    assert(block != NULL);
    assert(denominator > 0);

    Seq_T YPbPrVals = Seq_new(Seq_length(block));

    /* converts the Pnm_rgb structs to Y/Pb/Pr values */
    for (int i = 0; i < Seq_length(block); i++) {
        Pnm_rgb pixel = Seq_get(block, i);
        struct YPbPr *yppConvert = malloc(sizeof(struct YPbPr));
        assert(yppConvert != NULL);

        *yppConvert = rgbToYpp(pixel, denominator);
        Seq_addhi(YPbPrVals, yppConvert);
    }

    /* Converts the Y/Pb/Pr values to a/b/c/d/average Pb/average Pr */
    struct FinalVals final = calcFinalVals(YPbPrVals);
    uint64_t word = bitpack(final);

    for (int j = 0; j < Seq_length(YPbPrVals); j++) {
        struct YPbPr *temp = Seq_get(YPbPrVals, j);
        free(temp);
    }

    Seq_free(&YPbPrVals);

    return word;
}

/* Function: rgbToYpp
 * Input: a Pnm_rgb pixel and a denominator
 * Does: Converts RGB values to Y/Pb/Pr values
 * Returns: a YPbPr struct with the values
 * Error checks: Checks that the denominator isn't negative.
 */
struct YPbPr rgbToYpp(Pnm_rgb pixel, int denominator)
{
    assert(denominator > 0);

    float red = (float)(pixel->red) / denominator;
    float green = (float)pixel->green / denominator;
    float blue = (float)pixel->blue / denominator;

    float y = (0.299 * red + 0.587 * green + 0.144 * blue);
    float Pb = (-0.168736 * red - 0.331264 * green + 0.5 * blue);
    float Pr = (0.5 * red - 0.418688 * green - 0.081312 * blue);

    /* clamping the values if they are too large or too small */
    checkVal(&y, 1, 0);
    checkVal(&Pb, 0.5, -0.5);
    checkVal(&Pr, 0.5, -0.5);

    struct YPbPr conversion;

    conversion.y = y;
    conversion.Pb = Pb;
    conversion.Pr = Pr;

    return conversion;
}

/* Function: calcFinalVals
 * Input: A sequence of Y/Pb/Pr values
 * Does: Calculates the a/b/c/d/average Pb/average Pr values for the block
 * Returns: a struct containing these values
 * Error checks: Checks that the sequence given and the value given are not
 *               NULL.
 */
struct FinalVals calcFinalVals(Seq_T YPbPrVals)
{
    assert(YPbPrVals != NULL);

    int length = Seq_length(YPbPrVals);
    float y[length];
    float PbSum = 0;
    float PrSum = 0;

    /* calculates the average Pb and Pr values for the block */
    for (int i = 0; i < Seq_length(YPbPrVals); i++) {
        struct YPbPr *value = Seq_get(YPbPrVals, i);
        assert(value != NULL);
        y[i] = value->y;
        PbSum = PbSum + value->Pb;
        PrSum = PrSum + value->Pr;
    }

    float avgPb = PbSum / length;
    float avgPr =  PrSum / length;

    /* clamping the values if they are too large/too small */
    checkVal(&avgPb, 0.5, -0.5);
    checkVal(&avgPr, 0.5, -0.5);

    float a = (y[3] + y[2] + y[1] + y[0]) / (float)length;
    float b = (y[3] + y[2] - y[1] - y[0]) / (float)length;
    float c = (y[3] - y[2] + y[1] - y[0]) / (float)length;
    float d = (y[3] - y[2] - y[1] + y[0]) / (float)length;

    /* clamping the values if they are too large/too small */
    checkVal(&a, 1, 0);
    checkVal(&b, 0.5, -0.5);
    checkVal(&c, 0.5, -0.5);
    checkVal(&d, 0.5, -0.5);

    struct FinalVals final = convertFinalVals(a, b, c, d, avgPb, avgPr);

    return final;
}

/* Function: convertFinalVals
 * Input: the a/b/c/d/average Pb/average Pr values for a block
 * Does: Converts the values into integers to be bitpacked
 * Returns: a FinalVals struct with the integers to be bitpacked for that block
 * Error checks: None
 */
struct FinalVals convertFinalVals(float a, float b, float c, float d,
                                            float avgPb, float avgPr)
{
    struct FinalVals final;

    /* clamping the values if they are too large/too small */
    checkVal(&b, 0.3, -0.3);
    checkVal(&c, 0.3, -0.3);
    checkVal(&d, 0.3, -0.3);

    final.avgPb = Arith40_index_of_chroma(avgPb);
    final.avgPr = Arith40_index_of_chroma(avgPr);
    final.a = round(a * 63);
    final.b = round(b * 103);
    final.c = round(c * 103);
    final.d = round(d * 103);

    return final;
}

/* Function: bitpack
 * Input: FinalVals struct
 * Does: Bitpacks the integers into a 32-bit compressed codeword
 * Returns: the compressed codeword
 * Error checks: None
 */
uint64_t bitpack(struct FinalVals final)
{
    uint64_t word = 0;
    
    uint64_t a = final.a;
    word = Bitpack_newu(word, 6, 26, a);

    int64_t b = final.b;
    word = Bitpack_news(word, 6, 20, b);

    int64_t c = final.c;
    word = Bitpack_news(word, 6, 14, c);
    
    int64_t d = final.d;
    word = Bitpack_news(word, 6, 8, d);
    
    uint64_t pb = final.avgPb;
    word = Bitpack_newu(word, 4, 4, pb);
    
    uint64_t pr = final.avgPr;
    word = Bitpack_newu(word, 4, 0, pr);
    
    return word;
}

/* Function: checkVal
 * Input: a pointer to a float and the bounds of the number
 * Does: clamps the value to either the upper or lower bound if it is out of
 *       scope.
 * Returns: Nothing
 * Error checks: Checks that the float x doesn't equal NULL.
 */
void checkVal(float *x, float upper, float lower)
{
    assert(x != NULL);

    if (*x > upper) {
        *x = upper;
    }
    if (*x < lower) {
        *x = lower;
    }
}