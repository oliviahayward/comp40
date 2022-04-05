/**************************************************************
 *                      um_execute.h
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
#include "um_segment.h"
#include "um_memory.h"

#ifndef EXECUTE
#define EXECUTE

/* Divides the program up into instructions and calls the relevant function */
void execute_um(segment_t program);

/* If register C doesn't equal 0, then the value of register A equals
 * value of register B
 */
void conditional_move(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc);

/* Adds the value at register B to register C mod 2^32 and stores the sum
 * in register A
 */
void addition(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc);

/* Multiplies the value at register B and the value at register C mod 
 * 2^32 and stores the product in register A
 */
void multiplication(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc);

/* Divide the value in register B by the value in register C and stores
 * the result in register A
 */
void division(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc);

/* Performs a bitwise AND operation on the values in register B and C. It 
 * then gets the opposite of this function and stores that value in 
 * register A
 */
void bitwise_nand(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc);

/* Stops the program and frees all memory */
void halt(mem_t memory);

/* Outputs the value at register C */
void output(mem_t memory, uint32_t rc);

/* Takes in a char as input and stores it in register C */
void input(mem_t memory, FILE *in, uint32_t rc);

/* If the value at register B doesn't equal 0, then duplicate the segment at
 * the value of register B and setg that as the new segment 0. Set the program 
 * counter to the register C - 1's place
 */
void load_instruction(mem_t memory, uint32_t rb, uint32_t rc);

#endif