#include <stdio.h>
#include <stdlib.h>
#include "um_emulator.h"
#include <stdbool.h>
#include "um_instructions.h"

#define WORD_WIDTH 32
#define OP_WIDTH 4
#define REG_WIDTH 3
#define OP_SHIFT 28
#define REG_RIGHT_SHIFT 29
#define OP13_VAL_SHIFT 7
#define RA_SHIFT 23
#define RB_SHIFT 26
#define RC_SHIFT 29


void run_um(mem_t memory)
{
    // fprintf(stderr, "at %s (%d)\n", __FILE__, __LINE__);
    // printf("%d\n", memory->segment0[0]);
    uint32_t prog_counter = 0;
    bool run = true;
    while (run) {
        read_instructions(&prog_counter, memory);
    }
        // fprintf(stderr, "at %s (%d)\n", __FILE__, __LINE__);
}

void read_instructions(uint32_t *prog_counter, mem_t memory)
{
    // int seg0size
    // sizeof(memory->segment0)/sizeof(uint32_t)
    // printf("%d\n", memory->segment0[0]);

    if (prog_counter == NULL) {
        free_mem(memory);
        exit(0);
    }

    // fprintf(stderr, "at %s (%d)\n", __FILE__, __LINE__);
    /* get instruction and op_code */
    uint32_t word = memory->segment0[*prog_counter];
    //printf("word: %d\n", word);
    // fprintf(stderr, "at %s (%d)\n", __FILE__, __LINE__);
    int OP_code = word >> OP_SHIFT;
    //printf("opcode: %d\n", OP_code);
    assert(OP_code >= 0 && OP_code <= 13);
    if (OP_code != 12) {
        *prog_counter = *prog_counter + 1;
    }

    unsigned ra_13 = 0;
    unsigned value = 0;
    int ra = 0;
    int rb = 0;
    int rc = 0;
    /* handle op_code = 13 case */
    if (OP_code == 13) {
        ra_13 = (word << OP_WIDTH) >> REG_RIGHT_SHIFT;
        value = (word << OP13_VAL_SHIFT) >> OP13_VAL_SHIFT;
    }else {
        ra = (word << RA_SHIFT) >> REG_RIGHT_SHIFT;
        rb = (word << RB_SHIFT) >> REG_RIGHT_SHIFT;
        rc = (word << RC_SHIFT) >> REG_RIGHT_SHIFT;
    }

    execute_instructions(&prog_counter, memory, OP_code, ra_13,
                        value, ra, rb, rc);

    // fprintf(stderr, "at %s (%d)\n", __FILE__, __LINE__);
}

void execute_instructions(uint32_t **prog_counter, mem_t memory,
    int OP_code, unsigned ra_13, unsigned value, int ra, int rb,
    int rc)
{
        // fprintf(stderr, "at %s (%d)\n", __FILE__, __LINE__);
    // if (OP_code == 0) {
    //     printf("op code 0   prog counter: %d\n", **prog_counter);
    //     conditional_move(memory, ra, rb, rc);
    // } else
    if (OP_code == 7) {
        printf("halt code\n");
        exit(0);
        halt(memory);
    }
    (void) ra;
    (void) rb;
    (void) rc;
    (void) prog_counter;
    (void) ra_13;
    (void) value;
        // fprintf(stderr, "at %s (%d)\n", __FILE__, __LINE__);
}