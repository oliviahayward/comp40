/* Olivia Hayward
 * Kate Epifanio
 * COMP40
 * um_driver.c
 * 11/19/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "um_memory.h"
#include "um_emulator.h"

int main(int argc, char* argv[])
{
    //TODO: check how many files are allowed
    //TODO: check if stdin is allowed
    assert(argc == 2);
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Invalid file name.\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    mem_t memory = init_mem(fp, filename);
    run_um(memory);
    free_mem(memory);
    fclose(fp);
}