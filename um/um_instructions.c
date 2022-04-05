/* Olivia Hayward
 * Kate Epifanio
 * um_instructions.c
 * HW6 COMP40
 * 11/21/2020
 */

#include <stdlib.h>
#include <stdio.h>
#include "um_instructions.h"
#include "seq.h"

/******** MEMORY MANAGEMENT FUNCTIONS ********/

// void map_segment(mem_t memory, int rb, int rc)
// {
// 	// uint32_t new_segment[rc] = malloc(sizeof(uint32_t) *
//     //                            memory->register_vals[rc - 1]);
//     //
// 	// for (int i = 0; i < rc; i++) {
// 	// 	new_segment[i] = 0;
// 	// }
//     //
// 	// segment_list[rb] = new_segment;
//     (void) memory;
//     (void) rb;
//     (void) rc;
// }
//
// void unmap_segment(mem_t memory, int rc)
// {
// 	Seq_addhi(openIDS, &segment_list[rc]);
// 	segment_list[rc] = NULL;
// }
//
// void segmented_load(mem_t memory, int ra, int rb, int rc)
// {
// 	segment_list[rb] = curr_segment_ptr;
// 	*(curr_segment_ptr) = curr_segment;
// 	memory->register_vals[ra - 1] = curr_segment[rc];
// }
//
// void segmented_store(mem_t memory, int ra, int rb, int rc)
// {
// 	segment_list[ra] = curr_segment_ptr;
// 	*(curr_segment_ptr) = curr_segment;
// 	curr_segment[rb] = memory->register_vals[rc - 1];
// }
//
// void load_program(mem_t memory, int rb, int rc)
// {
// 	segment_list[rb] = curr_segment_ptr;
// 	segment_list[0] = curr_segment_ptr;
// 	*(curr_segment_ptr) = curr_segment;
// 	program_counter = curr_segment[rc];
// }
//
// /******** ARITHMETIC & DATA FUNCTIONS ********/
//
// void add(mem_t memory, int ra, int rb, int rc)
// {
// 	uint32_t sum = (memory->register_vals[rb - 1] +
//                     memory->register_vals[rc - 1]) % (1 << 32);
// 	memory->register_vals[ra - 1] = sum;
// }
//
// void multiply(mem_t memory, int ra, int rb, int rc)
// {
// 	uint32_t total = (memory->register_vals[rb - 1] *
//                       memory->register_vals[rc - 1]) % (1 << 32);
// 	memory->register_vals[ra - 1] = total;
// }
//
// void divide(mem_t memory, int ra, int rb, int rc)
// {
// 	assert(rc != 0);
// 	uint32_t total = (memory->register_vals[rb - 1] /
//                       memory->register_vals[rc - 1]);
// 	memory->register_vals[ra - 1] = total;
// }
//
// void load_value(mem_t memory, int ra, uint32_t val)
// {
// 	memory->register_vals[ra - 1] = val;
// }
//
void conditional_move(mem_t memory, int ra, int rb, int rc)
{
	if (memory->register_vals[rc] != 0) {
		memory->register_vals[ra] = memory->register_vals[rb];
	}
}
//
// /******** LOGICAL FUNCTIONS ********/
//
// // check if this is right
// void bitwise_nand(mem_t memory, int ra, int rb, int rc)
// {
// 	uint32_t value = (memory->register_vals[rb - 1] &
//                       memory->register_vals[rc - 1]);
// 	memory->register_vals[ra - 1] = ~value;
// }
//
// /******** I/O FUNCTIONS ********/
//
// void input(mem_t memory, int rc, FILE *fp)
// {
// 	int value;
// 	//1 byte maximum
// 	fgets(value, 1, fp);
// 	assert(value <= 255 && value >= 0);
// 	memory->register_vals[rc - 1] = value;
// }
//
// void output(mem_t memory, int rc)
// {
// 	assert(memory->register_vals[rc - 1] <= 255 &&
//               memory->register_vals[rc - 1] >= 0);
// 	printf("%d\n", memory->register_vals[rc - 1]);
// }
//
// /******** MISC FUNCTIONS ********/
//
void halt(mem_t memory)
{
    free_mem(memory);
	exit(0);
}