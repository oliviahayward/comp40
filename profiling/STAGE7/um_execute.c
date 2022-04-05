/**************************************************************
 *                      um_execute.c
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
#include "um_execute.h"
#include "um_memory.h"
#include <stdbool.h>
#include "um_segment.h"

#define OP_SHIFT 28
#define OP_WIDTH 4
#define REG_RIGHT_SHIFT 29
#define OP13_VAL_SHIFT 7
#define RA_SHIFT 23
#define RB_SHIFT 26
#define RC_SHIFT 29

/* Function: execute_um
 * Input: A segment_t struct
 * Does: Divides the program up into instructions and then calls the correct
 *       instruction function
 * Returns: Nothing
 */
void execute_um(segment_t program)
{
    mem_t memory = init_mem(program);
    assert(memory);
    bool run = true;

    uint32_t ra;
    uint32_t rb;
    uint32_t rc;
    uint32_t value;
    uint32_t word;
    while (run) {
        int count = memory->program_counter;
        word = (memory->segments[0]->instructions)[count];

        /* get op code from word */
        uint32_t op_code = word >> OP_SHIFT;

        /* get remaining values */
        if (op_code == 13) {
            ra = (word << OP_WIDTH) >> REG_RIGHT_SHIFT;
            value = (word << OP13_VAL_SHIFT) >> OP13_VAL_SHIFT;
            memory->eight_regs[ra] = value;
        } else {
            ra = (word << RA_SHIFT) >> REG_RIGHT_SHIFT;
            rb = (word << RB_SHIFT) >> REG_RIGHT_SHIFT;
            rc = (word << RC_SHIFT) >> REG_RIGHT_SHIFT;

            /* call appropriate instructions for each op code */
            if (op_code == 0) {
                conditional_move(memory, ra, rb, rc);
            } else if (op_code == 1) {
                int rb_index = memory->eight_regs[rb];
                memory->eight_regs[ra] = (memory->segments
                    [rb_index]->instructions)[memory->eight_regs[rc]];
            } else if (op_code == 2) {
                segmented_store(memory, memory->eight_regs[ra],
                    memory->eight_regs[rb], memory->eight_regs[rc]);
            } else if (op_code == 3) {
                addition(memory, ra, rb, rc);
            } else if (op_code == 4) {
                multiplication(memory, ra, rb, rc);
            } else if (op_code == 5) {
                division(memory, ra, rb, rc);
            } else if (op_code == 6) {
                bitwise_nand(memory, ra, rb, rc);
            } else if (op_code == 7) {
                halt(memory);
            } else if (op_code == 8) {
                memory->eight_regs[rb] = map_segment(memory,
                                    memory->eight_regs[rc]);
            } else if (op_code == 9) {
                unmap_segment(memory, memory->eight_regs[rc]);
            } else if (op_code == 10) {
                output(memory, rc);
            } else if (op_code == 11) {
                input(memory, stdin, rc);
            } else if (op_code == 12) {
                load_instruction(memory, rb, rc);
            }
        }
        (memory->program_counter)++;
    }
}

/* Function: Conditional_move
 * Input: Mem_t struct, registers A, B, and C
 * Does: If register c doesn't equal 0, then the value of register A = value
 *       of register B
 * Returns: Nothing
 */
void conditional_move(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    if (memory->eight_regs[rc] != 0) {
        memory->eight_regs[ra] = memory->eight_regs[rb];
    }
}

/* Function: addition
 * Input: Mem_t struct, registers A, B, and C
 * Does: Adds the value at register B to register C mod 2^32 and stores the
 *       sum in register A
 * Returns: None
 */
void addition(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = (memory->eight_regs[rb]
        + memory->eight_regs[rc]) % 4294967296;
}

/* Function: multiply
 * Input: Mem_t struct, registers A, B, and C
 * Does: Multiplies the value at register B and the value at register C mod
 *       2^32 and stores the product in register A
 * Returns: None
 */
void multiplication(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = (memory->eight_regs[rb]
        * memory->eight_regs[rc]) % 4294967296;
}

/* Function: division
 * Input: Mem_t struct, registers A, B, and C
 * Does: Divide the value in register B by the value in register C and stores
 *       the result in register A
 * Returns: None
 */
void division(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = (memory->eight_regs[rb]
        / memory->eight_regs[rc]);
}

/* Function: bitwise_nand
 * Input: Mem_t struct, registers A, B, and C
 * Does: Performs a bitwise AND operation on the values in register B and C.
 *       It then gets the opposite of this function and stores that value in
 *       register A.
 * Returns: None
 */
void bitwise_nand(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = ~((memory->eight_regs[rb])
                        & (memory->eight_regs[rc]));
}

/* Function: halt
 * Input: Mem_t struct, registers A, B, and C
 * Does: Stops the program and frees all of the memory.
 * Returns: None
 */
void halt(mem_t memory)
{
    free_mem(memory);
    exit(0);
}

/* Function: output
 * Input: Mem_t struct, registers A, B, and C
 * Does: Outputs the value at register C
 * Returns: None
 */
void output(mem_t memory, uint32_t rc)
{
    assert(rc <= 255);
    putchar(memory->eight_regs[rc]);
}

/* Function: input
 * Input: Mem_t struct, registers A, B, and C
 * Does: Takes in a char as input and stores it in register C
 * Returns: None
 */
void input(mem_t memory, FILE *in, uint32_t rc)
{
    uint32_t instruction = getc(in);
    if (instruction == (uint32_t) EOF) {
        instruction = -1;
    }
    memory->eight_regs[rc] = instruction;
}

/* Function: load_instruction
 * Input: Mem_t struct, registers A, B, and C
 * Does: If the value at register B doesn't equal 0, then duplicate the
 *       segment at the value of register B and set that as the new segment 0.
 *       Set the program counter to the register C - 1's place
 * Returns: None
 */
void load_instruction(mem_t memory, uint32_t rb, uint32_t rc)
{
    if (memory->eight_regs[rb] != 0) {
        int rb_index = memory->eight_regs[rb];
        segment_t instr_holder = (memory->segments)[rb_index];
        uint32_t seg_length = instr_holder->length;
        segment_t new_segment = init_seg(seg_length);
        for (uint32_t i = 0; i < seg_length; i++) {
            store_instruction(get_segment(i, instr_holder), new_segment);
        }
        memory->segments[0] = new_segment;
    }
    memory->program_counter = memory->eight_regs[rc] - 1;
}
