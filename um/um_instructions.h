/* Olivia Hayward
 * Kate Epifanio
 * um_instructions.c
 * HW6 COMP40
 * 11/21/2020
 */
#include <stdlib.h>
#include <stdio.h>
#include "um_memory.h"

// /******** MEMORY MANAGEMENT FUNCTIONS ********/
// void map_segment(mem_t memory, int rb, int rc);
// void unmap_segment(mem_t memory, int rc);
// void segmented_load(mem_t memory, int ra, int rb, int rc);
// void segmented_store(mem_t memory, int ra, int rb, int rc);
// void load_program(mem_t memory, int rb, int rc);
//
// /******** ARITHMETIC & DATA FUNCTIONS ********/
// void add(mem_t memory, int ra, int rb, int rc);
// void multiply(mem_t memory, int ra, int rb, int rc);
// void divide(mem_t memory, int ra, int rb, int rc);
// void load_value(mem_t memory, int ra, uint32_t val);
void conditional_move(mem_t memory, int ra, int rb, int rc);

// /******** LOGICAL FUNCTIONS ********/
// void bitwise_nand(mem_t memory, int ra, int rb, int rc);
//
// /******** I/O FUNCTIONS ********/
// void input(mem_t memory, int rc, FILE *fp);
// void output(mem_t memory, int rc);
//
// /******** MISC FUNCTIONS ********/
void halt(mem_t memory);