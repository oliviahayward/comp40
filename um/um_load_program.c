/**************************************************************
 *                      um_load_program.c
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
#include "um_load_program.h"
#include <bitpack.h>
#include <stdint.h>
#include <sys/stat.h>

/* Function: Load_program
 * Input: filename string, file pointer
 * Does: It reads the file with the instructions in it and determines
 *       the number of instructions as well as stores them to be read
 * Returns: A segment_t struct
 */
segment_t load_program(char *filename, FILE *fp)
{
    /* determine size of file before reading */
    struct stat file_info;
    stat(filename, &file_info);
    int file_size = file_info.st_size;

    /* check whether file size is valid */
    if (file_size == 0 || file_size % 4 != 0) {
        fprintf(stderr, "Invalid file format.\n");
        exit(EXIT_FAILURE);
    }

    /* create segment for progam of proper size */
    int seg_size = file_size / 4;
    segment_t program = init_seg(seg_size);

    /* read contents of file */
    int c1;
    while ((c1 = getc(fp)) != EOF) {
        uint32_t instruction = new_instruction(fp, c1);
        store_instruction(instruction, program);
    }
    return program;
}

/* Function: new_instruction
 * Input: file pointer, integer of the first byte of the instruction
 * Does: takes the instruction from the file and formats it to be a
 *       UM-readable instruction
 * Returns: the uint32_t formatted instruction
 */
uint32_t new_instruction(FILE *fp, int c1)
{
    int c2 = getc(fp);
    int c3 = getc(fp);
    int c4 = getc(fp);

    uint64_t new_instr = 0;
    new_instr = Bitpack_newu(new_instr, 8, 24, c1);
    new_instr = Bitpack_newu(new_instr, 8, 16, c2);
    new_instr = Bitpack_newu(new_instr, 8, 8, c3);
    new_instr = Bitpack_newu(new_instr, 8, 0, c4);

    return new_instr;
}