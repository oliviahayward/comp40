#include <stdio.h>
#include <stdlib.h>
#include "um_memory.h"

void run_um(mem_t memory);

void read_instructions(uint32_t *prog_counter, mem_t memory);

void execute_instructions(uint32_t **prog_counter, mem_t memory,
                        int OP_code, unsigned ra_13, unsigned value,
                        int ra, int rb, int rc);