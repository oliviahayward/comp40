/**************************************************************
 *                          um_segment.h
 *
 *    Assignment: um
 *    Authors: Olivia Hayward (ohaywa02)
               Katherine Epifanio (kepifa01)
 *    Date: 11.25.20
 *
 **************************************************************/

#ifndef SEGMENT
#define SEGMENT

#include <seq.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Seq_T *segment_t;

/* Make a sequence of segments */
segment_t init_seg(int quantity);

/* Frees the segments */
void free_seg(segment_t segment); 

/* Adds the instruction to a sequence of instructions */
void store_instruction(uint32_t instruction, segment_t program);

/* Returns the instruction at that segment with that index */
uint32_t get_segment(uint32_t index, segment_t segment);

/* Stores the value at that segment at that index */
void put_segment(uint32_t index, segment_t segment, uint32_t value);


#endif