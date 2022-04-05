/**************************************************************
 *                      um_load_program.h
 *
 *    Assignment: um
 *    Authors: Olivia Hayward (ohaywa02)
               Katherine Epifanio (kepifa01)
 *    Date: 11.25.20
 *
 *    Summary:
 **************************************************************/

#ifndef LOAD_PROGRAM
#define LOAD_PROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "um_segment.h"

/* reads the file and determines number of instructions and stores them */
segment_t load_program(char *filename, FILE *fp);

/* takes the instruction from the file and formats to be readable by the UM */
uint32_t new_instruction(FILE *fp, int c1);

#endif
