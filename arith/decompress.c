/* Olivia Hayward and Meghan Kloud
 * ohaywa02 and mkloud01
 * HW4 arith
 * decompress.c
 * 10/26/2020
 */

#include "decompress.h"

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

Seq_T yppToRgb(Seq_T newYpp);
Seq_T unFinalVals(struct FinalVals final);
struct FinalVals unbitpack(uint64_t word);
void checkLimit(float *x, float upper, float lower);

/* Function: decompress
 * Does: Acts as the interface for decompression. Calls all of the
 *       decompression steps in order. See helper function contracts for more
 *       details.
 * Input: A compressed 32-bit codeword
 * Returns: a sequence of Pnm_rgb structs
 * Error checks: If the compressed codeword is less than or equal to 0, it is a
 *               CRE.
 */
Seq_T decompress(uint64_t compressed)
{
    assert(compressed > 0);
    struct FinalVals decompressed = unbitpack(compressed);
    Seq_T newYpp = unFinalVals(decompressed);
    Seq_T newVals = yppToRgb(newYpp);
    return newVals;
}

/* Function: yppToRgb
 * Input: a sequence of Y/Pb/Pr structs
 * Does: Converts Y/Pb/Pr values to RGB values
 * Returns: a sequence struct with the values
 * Error checks: Checks that neither oldVal nor newRgb are negative. Checks
 *               that the red, green, and blue are in the appropriate ranges.
 */
Seq_T yppToRgb(Seq_T newYpp)
{
    Seq_T newRgbVals = Seq_new(Seq_length(newYpp));
    for (int i = 0; i < Seq_length(newYpp); i++) {
        struct YPbPr *oldVal = Seq_get(newYpp, i);
        assert(oldVal != NULL);
        struct Pnm_rgb *newRgb = malloc(sizeof(struct Pnm_rgb));
        assert(newRgb != NULL);

        float red = 1.0 * oldVal->y + 0.0 * oldVal->Pb + 1.402 * oldVal->Pr;
        float green = 1.0 * oldVal->y - 0.344136 * oldVal->Pb - 0.714136 * oldVal->Pr;
        float blue = 1.0 * oldVal->y + 1.772 * oldVal->Pb + 0.0 * oldVal->Pr;

        checkLimit(&red, 1, 0);
        checkLimit(&green, 1, 0);
        checkLimit(&blue, 1, 0);

        newRgb->red = red * 255;
        newRgb->green = green * 255;
        newRgb->blue = blue * 255;
        Seq_addhi(newRgbVals, newRgb);
    }

    for (int j = 0; j < Seq_length(newYpp); j++){
        struct YPbPr *temp = Seq_get(newYpp, j);
        free(temp);
    }
    
    Seq_free(&newYpp);
    
    return newRgbVals;
}

/* Function: unFinalVals
 * Input: A FinalVals struct containing the  a/b/c/d/average Pb/average Pr
 *        values for the block
 * Does: Calculates the Y/Pb/Pr values for each of the pixels in the block
 * Returns: A sequence of Y/Pb/Pr values
 * Error checks: Checks that the y, Pb, and Pr values are in the correct
 *               ranges. Checks that new has been allocated correctly.
 */
Seq_T unFinalVals(struct FinalVals final)
{
    Seq_T newVals = Seq_new(4);
    float y[4];

    float a = ((float)final.a) / 63.0;
    float b = ((float)final.b) / 103.0;
    float c = ((float)final.c) / 103.0;
    float d = ((float)final.d) / 103.0;

    y[0] = a - b - c + d;
    y[1] = a - b + c - d;
    y[2] = a + b - c - d;
    y[3] = a + b + c + d;

    for(int i = 0; i < 4; i++) {
        struct YPbPr *new = malloc(sizeof(struct YPbPr));
        assert(new != NULL);

        checkLimit(&y[i], 1, 0);
        new->y = y[i];

        float Pb = Arith40_chroma_of_index(final.avgPb);
        float Pr = Arith40_chroma_of_index(final.avgPr);
        
        checkLimit(&Pb, 0.5, -0.5);
        checkLimit(&Pr, 0.5, -0.5);
        
        new->Pb = Pb;
        new->Pr = Pr;
        
        Seq_addhi(newVals, new);
    }

    return newVals;
}

/* Function: unbitpack
 * Input: Compressed 32-bit codeword
 * Does: Unpacks the integers from a 32-bit compressed codeword into a
 *       FinalVals struct
 * Returns: FinalVals struct
 * Error checks: None
 */
struct FinalVals unbitpack(uint64_t word)
{
    struct FinalVals final;

    uint64_t a = Bitpack_getu(word, 6, 26);
    final.a = a;

    int64_t b = Bitpack_gets(word, 6, 20);
    final.b = b;

    int64_t c = Bitpack_gets(word, 6, 14);
    final.c = c;

    int64_t d = Bitpack_gets(word, 6, 8);
    final.d = d;

    uint64_t pb = Bitpack_getu(word, 4, 4);
    final.avgPb = pb;

    uint64_t pr = Bitpack_getu(word, 4, 0);
    final.avgPr = pr;

    return final;
}

/* Function: checkLimit
 * Input: a pointer to a float and the bounds of the number
 * Does: clamps the value to either the upper or lower bound if it is out of
 *       scope.
 * Returns: Nothing
 * Error checks: Checks that the float x doesn't equal NULL.
 */
void checkLimit(float *x, float upper, float lower)
{
    assert(x != NULL);
    if (*x > upper) {
        *x = upper;
    }
    if (*x < lower) {
        *x = lower;
    }
}