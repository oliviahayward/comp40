/**************************************************************
 *                      um_memory.c
 *
 *    Assignment: um
 *    Authors: Olivia Hayward (ohaywa02)
               Katherine Epifanio (kepifa01)
 *    Date: 11.25.20
 *
 *    Summary:
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "um_memory.h"
#include <stdbool.h>

/* Function: init_mem
 * Input: A segment_t program
 * Does: Instantiates a mem_t struct
 * Returns: mem_t struct
 * Error check: Asserts that memory has been allocated
 */
mem_t init_mem(segment_t program)
{
    mem_t memory = malloc(sizeof(*memory));
    assert(memory);

    memory->open_segments = Stack_new();
    memory->program_counter = 0;
    memory->eight_regs = malloc(8 * sizeof(uint32_t));
    for (int i = 0; i < 8; i++) {
        memory->eight_regs[i] = 0;
    }
    memory->segments = malloc(sizeof(segment_t) * 500);
    assert(memory->segments);
    memory->segments[0] = program;
    memory->length = 1;
    memory->capacity = 500;
    return memory;
}

/* Function: segmented_load
 * Input: mem_t struct, register B int, register C int
 * Does: Loads in a segment and the value at a certain point of that segment
 * Returns: The value of the segment at that offset
 * Error check: asserts that the mem_t struct exists
 */
// uint32_t segmented_load(mem_t memory, uint32_t rb, uint32_t rc)
// {
//     assert(memory);
//     segment_t seg2get = memory->segments[rb];
//     uint32_t load = get_segment(rc, seg2get);
//     return load;
// }

/* Function: segmented_store
 * Input: mem_t struct, register A int, register B int, register C int
 * Does: Stores a value at the specified segment and offset
 * Returns: Nothing
 * Error check: asserts that the mem_t struct exists
 */
void segmented_store(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    assert(memory);
    put_segment(rb, memory->segments[ra], rc);
    return;
}

/* Function: free_mem
 * Input: mem_t struct
 * Does: frees all the memory allocated when the mem_t struct was created
 * Returns: Nothing
 * Error checks: asserts that the mem_t struct exists
 */
void free_mem(mem_t memory)
{
    assert(memory);
    int seg_length = memory->length;
    for (int i = 0; i < seg_length; i++) {
        free_seg(memory->segments[i]);
    }
    free(memory->segments);
    free(memory->eight_regs);
    Stack_free(&((memory)->open_segments));
    free(memory);
}

/* Function: map_segment
 * Input: mem_t struct, register C int
 * Does: It adds a segment of memory with space for the specified number of codewords. If there is a previously unmapped segment available, the program will use that one first. If not, it will allocate a new segment of memory.
 * Returns: the index of the mapped segment
 */
uint32_t map_segment(mem_t memory, uint32_t rc)
{
    assert(memory);
    segment_t segment = init_seg(rc);
    for (uint32_t i = 0; i < rc; i++) {
        store_instruction(0, segment);
    }

    uint32_t rb_index;
    /* check for available segment */
    if (Stack_empty(memory->open_segments) == false) {
        rb_index = (int)(uintptr_t) Stack_pop(memory->open_segments);
        free_seg(memory->segments[rb_index]);
        memory->segments[rb_index] = segment;
    } else {
        put_seg_in_memory(segment, memory);
        rb_index = memory->length - 1;
    }

    return rb_index;
}

void put_seg_in_memory(segment_t segment, mem_t memory)
{
    assert(memory);

    /* check if more space is needed in memory->segments */
    int counter = 0;
    if (memory->capacity == memory->length) {
        counter++;
        int new_capacity = memory->capacity * 2;
        segment_t *new_segs = malloc(sizeof(segment_t) * new_capacity);
        for (int i = 0; i < memory->length; i++) {
            new_segs[i] = memory->segments[i];
        }
        free(memory->segments);
        memory->segments = new_segs;
        memory->capacity = new_capacity;
    }

    /* put the segment in the memory struct */
    memory->segments[memory->length] = segment;
    memory->length += 1;
    return;
}

/* Function: unmap_segment
 * Input: mem_t struct, register C int
 * Does: Puts the segment in the open_segments list
 * Returns: Nothing
 * Error checks: asserts that the mem_t struct exists
 */
void unmap_segment(mem_t memory, uint32_t rc)
{
    assert(memory);
    Stack_push(memory->open_segments, (void*) (uintptr_t) rc);
}
