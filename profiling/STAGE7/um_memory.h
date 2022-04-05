/**************************************************************
 *                      um_memory.h
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
 #include <stdint.h>
 #include "um_segment.h"
 #include <assert.h>
 #include <stdbool.h>

 #ifndef MEMORY
 #define MEMORY

typedef struct stack_t {
    uint32_t *stack;
    int capacity;
    int length;
} *stack_t;


stack_t init_stack();
void free_stack(stack_t stack);
void stack_push(stack_t stack, uint32_t index);
uint32_t stack_pop(stack_t stack);
bool stack_isempty(stack_t stack);

typedef struct mem_t {
    int program_counter;
    stack_t open_segments;
    uint32_t *eight_regs;
    segment_t *segments;
    int capacity;
    int length;
} *mem_t;

/* Instatiates a mem_t struct */
mem_t init_mem(segment_t program);

/* Frees all the memory allocated when the mem_t struct was created */
void free_mem(mem_t memory);

/******** MEMORY MANAGEMENT FUNCTIONS ********/
/* Loads in a segment and the value at the specified index of that segment */
// uint32_t segmented_load(mem_t memory, uint32_t rb, uint32_t rc);

/* Stores a value at that specified segment and index */
void segmented_store(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc);

/* Adds a segment of memory with space for the specified number of codewords */
uint32_t map_segment(mem_t memory, uint32_t rc);

/* Puts the segment in the open_segments list */
void unmap_segment(mem_t memory, uint32_t rc);

void put_seg_in_memory(segment_t segment, mem_t memory);

#endif
