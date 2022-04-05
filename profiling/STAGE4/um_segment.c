#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "um_segment.h"
#include <assert.h>

segment_t init_seg(int quantity)
{
    assert(quantity > 0);
    segment_t new_seg = malloc(sizeof(*new_seg));
    new_seg->size = quantity;
    new_seg->length = 0;
    new_seg->instructions = malloc(sizeof(uint32_t) * quantity);
    return new_seg;
}

void free_seg(segment_t segment)
{
    assert(segment);
    free(segment->instructions);
    free(segment);
    return;
}

void store_instruction(uint32_t instruction, segment_t program)
{
    assert(program);
    int size = program->size;
    int length = program->length;

    /* if more space is needed, double segment size */
    if (size == length) {
        int new_size = size * 2;
        uint32_t *new_instrs = realloc(program->instructions,
            sizeof(uint32_t) * new_size);
        free(program->instructions);
        program->instructions = new_instrs;
        program->size = new_size;
    }

    /* add instruction to the segment */
    program->instructions[length] = instruction;
    program->length += 1;
    return;
}

void put_segment(uint32_t index, segment_t segment, uint32_t value)
{
    assert(segment);
    assert(index < segment->length);
    segment->instructions[index] = value;
    return;
}

uint32_t get_segment(uint32_t index, segment_t segment)
{
    assert(segment);
    assert(index < segment->length);
    uint32_t value = segment->instructions[index];
    return value;
}
