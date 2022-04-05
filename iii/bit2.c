/* Olivia Hayward
Claudia Aranda Barrios
bit2.h
9/27/2020 */

#include "bit2.h"

#define B Bit2_T
struct B {
    Bit_T array;
    int height;
    int width;
};

B Bit2_new(int width, int height) {
    Bit_T bit_array = Bit_new((height * width));

    B BitArray = malloc(sizeof(struct B));

    BitArray->array = bit_array;
    BitArray->height = height;
    BitArray->width = width;

    return BitArray;
}

void Bit2_free(B *bitarray) {
    Bit_free(&(*bitarray)->array);
    free(*bitarray);
}

int Bit2_width(B bit2array) {
    return bit2array->width;
}

int Bit2_height(B bit2array) {
    return bit2array->height;
}

int Bit2_get(B bit2array, int col, int row) {
    int position = (bit2array->width * col) + row;
    assert(position < (bit2array->width * bit2array->height));


    int result = Bit_get(bit2array->array, position);
    assert(!(result > 1));
    assert(!(result < 0));

    return result;
}

int Bit2_put(B bit2array, int col, int row, int newValue) {
    int position = (bit2array->width * col) + row;
    assert(position < (bit2array->width * bit2array->height));

    int prev = Bit_put(bit2array->array, position, newValue);

    return prev;
}

void Bit2_map_row_major(B bit2array, void apply(int col, int row, B a, int value, void *p2), void *cl) {
    for (int row = 0; row < bit2array->height; row++) {
        for (int col = 0; col < bit2array->width; col++) {
            apply(col, row, bit2array, Bit2_get(bit2array, col, row), cl);
        }
    }
}

void Bit2_map_col_major(B bit2array, void apply(int col, int row, B a, int value, void *p2), void *cl) {
    for (int col = 0; col < bit2array->width; col++) {
        for (int row = 0; row < bit2array->height; row++) {
            apply(col, row, bit2array, Bit2_get(bit2array, col, row), cl);
        }
    }

}

void print_array(B bit2array) {
    int total_length = bit2array->height * bit2array->width;
    for (int i = 0; i < total_length; i++) {
        printf("%d ", Bit_get(bit2array->array, i));
    }
}