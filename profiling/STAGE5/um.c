/**************************************************************
 *                            um.c
 *
 *    Assignment: um
 *    Authors: Olivia Hayward (ohaywa02)
               Katherine Epifanio (kepifa01)
 *    Date: 11.25.20
 *
 *    Summary:
 *
 *          This file contains the main driver code for
 *          the um. It takes in the input file, passing
 *          it to um_load_program.h and passes the
 *          resulting program for execution to um_execute.h
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "um_load_program.h"
#include "um_execute.h"

int main(int argc, char *argv[])
{
     assert(argc == 2);
     FILE *fp = fopen(argv[1], "r");
     assert(fp != NULL);

     /* load program into a segment_t*/
     segment_t input_program = load_program(argv[1], fp);
     fclose(fp);

     /* pass program to get executed */
     execute_um(input_program);

     exit(0);
}