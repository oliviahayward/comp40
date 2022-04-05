/**************************************************************
 *                          um_segment.h
 *
 *    Assignment: um
 *    Authors: Olivia Hayward (ohaywa02)
 *             Katherine Epifanio (kepifa01)
 *
 **************************************************************/

#ifndef SEGMENT
#define SEGMENT

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct segment_t {
    uint32_t length;
    uint32_t *instructions;
    int size;
} *segment_t;


segment_t init_seg(int quantity);
void free_seg(segment_t segment);
void store_instruction(uint32_t instruction, segment_t program);
void put_segment(uint32_t index, segment_t segment, uint32_t value);
uint32_t get_segment(uint32_t index, segment_t segment);


#endif
