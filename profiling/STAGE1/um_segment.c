/**************************************************************
 *                       um_segment.c
 *
 *    Assignment: um
 *    Authors: Olivia Hayward (ohaywa02)
 *             Katherine Epifanio (kepifa01)
 *    Date: 11.25.20
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "um_segment.h"
#include <assert.h>

/* Function: init_seg
 * Input: the number of segments to create
 * Does: Makes a sequence of segments
 * Returns: the segment sequence
 */
segment_t init_seg(int quantity)
{
    segment_t new_seg = Seq_new(quantity);
    return new_seg;
}

/* Function: store_instruction
 * Input: instruction, the program
 * Does: adds the instruction to a sequence of instructions
 * Returns: Nothing
 */
void store_instruction(uint32_t instruction, segment_t program)
{
    Seq_addhi(program, (void*)(uintptr_t) instruction);
}

/* Function: get_segment
 * Input: index of the segment, segment sequence
 * Does: returns the instruction at that segment with that offset
 * Returns: returns the instruction at that segment with that offset
 */
uint32_t get_segment(uint32_t index, segment_t segment)
{
    return (uint32_t) (uintptr_t) Seq_get(segment, index);
}

/* Function: put_segment
 * Input: index, the sequence of segments, value
 * Does: Stores that value at that segment at that index/offset
 * Returns: Nothing
 */
void put_segment(uint32_t index, segment_t segment, uint32_t value)
{
    Seq_put(segment, index, (void*) (uintptr_t) value);
}

/* Function: free_seg
 * Input: Sequence of segments
 * Does: frees the segments
 * Returns: nothing
 * Error checks: checks that the sequence exists
 */
void free_seg(segment_t segment)
{
    assert(segment);
    Seq_free(&segment);
    return;
}