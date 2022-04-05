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
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <bitpack.h>

#define OP_SHIFT 28
#define OP_WIDTH 4
#define REG_RIGHT_SHIFT 29
#define OP13_VAL_SHIFT 7
#define RA_SHIFT 23
#define RB_SHIFT 26
#define RC_SHIFT 29

typedef struct stack_t {
    uint32_t *stack;
    int capacity;
    int length;
} *stack_t;

typedef struct segment_t {
    uint32_t length;
    uint32_t *instructions;
    int size;
} *segment_t;

typedef struct mem_t {
    int program_counter;
    stack_t open_segments;
    uint32_t *eight_regs;
    segment_t *segments;
    int capacity;
    int length;
} *mem_t;


static inline segment_t init_seg(int quantity)
{
    assert(quantity > 0);
    segment_t new_seg = malloc(sizeof(*new_seg));
    new_seg->size = quantity;
    new_seg->length = 0;
    new_seg->instructions = malloc(sizeof(uint32_t) * quantity);
    return new_seg;
}

static inline void free_seg(segment_t segment)
{
    assert(segment);
    free(segment->instructions);
    free(segment);
    return;
}

static inline void store_instruction(uint32_t instruction, segment_t program)
{
    assert(program);
    int size = program->size;
    int length = program->length;

    /* if more space is needed, double segment size */
    if (size == length) {
        int new_size = size * 2;
        uint32_t *new_instrs = realloc(program->instructions,
            sizeof(uint32_t) * new_size);
        free(program->instructions);
        program->instructions = new_instrs;
        program->size = new_size;
    }

    /* add instruction to the segment */
    program->instructions[length] = instruction;
    program->length += 1;
    return;
}

static inline void put_segment(uint32_t index, segment_t segment, uint32_t value)
{
    assert(segment);
    assert(index < segment->length);
    segment->instructions[index] = value;
    return;
}

static inline uint32_t get_segment(uint32_t index, segment_t segment)
{
    assert(segment);
    assert(index < segment->length);
    uint32_t value = segment->instructions[index];
    return value;
}

static inline stack_t init_stack()
{
    stack_t new_stack = malloc(sizeof(*new_stack));
    new_stack->stack = malloc(sizeof(uint32_t) * 500);
    new_stack->capacity = 500;
    new_stack->length = 0;
    return new_stack;
}

static inline void free_stack(stack_t stack)
{
    assert(stack);
    assert(stack->stack);
    free(stack->stack);
    free(stack);
    return;
}

static inline bool stack_isempty(stack_t stack)
{
    return (stack->length == 0);
}

static inline void stack_push(stack_t stack, uint32_t index)
{
    assert(stack);
    assert(stack->stack);

    /* check if more memory is necessary */
    if (stack->length == stack->capacity) {
        int new_size = stack->length * 2;
        uint32_t *new_stack = malloc(sizeof(uint32_t) * new_size);
        for (int i = 0; i < stack->length; i++) {
            new_stack[i] = stack->stack[i];
        }
        stack->capacity = stack->capacity * 2;
        free(stack->stack);
        stack->stack = new_stack;
    }

    stack->stack[stack->length] = index;
    stack->length += 1;
    return;
}

static inline uint32_t stack_pop(stack_t stack)
{
    assert(stack);
    assert(stack->stack);

    uint32_t val = stack->stack[stack->length - 1];
    stack->length = stack->length - 1;
    return val;
}

static inline mem_t init_mem(segment_t program)
{
    mem_t memory = malloc(sizeof(*memory));
    assert(memory);

    memory->open_segments = init_stack();
    memory->program_counter = 0;
    memory->eight_regs = malloc(8 * sizeof(uint32_t));
    for (int i = 0; i < 8; i++) {
        memory->eight_regs[i] = 0;
    }
    memory->segments = malloc(sizeof(segment_t) * 500);
    assert(memory->segments);
    memory->segments[0] = program;
    memory->length = 1;
    memory->capacity = 500;
    return memory;
}

static inline void segmented_store(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    assert(memory);
    put_segment(rb, memory->segments[ra], rc);
    return;
}

static inline void free_mem(mem_t memory)
{
    assert(memory);
    int seg_length = memory->length;
    for (int i = 0; i < seg_length; i++) {
        free_seg(memory->segments[i]);
    }
    free(memory->segments);
    free(memory->eight_regs);
    free_stack(memory->open_segments);
    free(memory);
}

static inline void put_seg_in_memory(segment_t segment, mem_t memory)
{
    assert(memory);

    /* check if more space is needed in memory->segments */
    int counter = 0;
    if (memory->capacity == memory->length) {
        counter++;
        int new_capacity = memory->capacity * 2;
        segment_t *new_segs = malloc(sizeof(segment_t) * new_capacity);
        for (int i = 0; i < memory->length; i++) {
            new_segs[i] = memory->segments[i];
        }
        free(memory->segments);
        memory->segments = new_segs;
        memory->capacity = new_capacity;
    }

    /* put the segment in the memory struct */
    memory->segments[memory->length] = segment;
    memory->length += 1;
    return;
}

static inline uint32_t map_segment(mem_t memory, uint32_t rc)
{
    assert(memory);
    segment_t segment = init_seg(rc);
    for (uint32_t i = 0; i < rc; i++) {
        store_instruction(0, segment);
    }

    uint32_t rb_index;
    /* check for available segment */
    if (stack_isempty(memory->open_segments) == false) {
        rb_index = (int)(uintptr_t) stack_pop(memory->open_segments);
        free_seg(memory->segments[rb_index]);
        memory->segments[rb_index] = segment;
    } else {
        put_seg_in_memory(segment, memory);
        rb_index = memory->length - 1;
    }

    return rb_index;
}

static inline void unmap_segment(mem_t memory, uint32_t rc)
{
    assert(memory);
    stack_push(memory->open_segments, rc);
}

static inline void load_instruction(mem_t memory, uint32_t rb, uint32_t rc)
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

static inline void input(mem_t memory, FILE *in, uint32_t rc)
{
    uint32_t instruction = getc(in);
    if (instruction == (uint32_t) EOF) {
        instruction = -1;
    }
    memory->eight_regs[rc] = instruction;
}

static inline void output(mem_t memory, uint32_t rc)
{
    assert(rc <= 255);
    putchar(memory->eight_regs[rc]);
}

static inline void halt(mem_t memory)
{
    free_mem(memory);
    exit(0);
}

static inline void bitwise_nand(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = ~((memory->eight_regs[rb])
                        & (memory->eight_regs[rc]));
}

static inline void division(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = (memory->eight_regs[rb]
        / memory->eight_regs[rc]);
}

static inline void multiplication(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = (memory->eight_regs[rb]
        * memory->eight_regs[rc]) % 4294967296;
}

static inline void addition(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    memory->eight_regs[ra] = (memory->eight_regs[rb]
        + memory->eight_regs[rc]) % 4294967296;
}

static inline void conditional_move(mem_t memory, uint32_t ra, uint32_t rb, uint32_t rc)
{
    if (memory->eight_regs[rc] != 0) {
        memory->eight_regs[ra] = memory->eight_regs[rb];
    }
}

static inline uint32_t new_instruction(FILE *fp, int c1)
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

static inline segment_t load_program(char *filename, FILE *fp)
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

static inline void execute_um(segment_t program)
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
