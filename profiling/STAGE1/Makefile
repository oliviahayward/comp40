# Makefile for um (Comp 40 Assignment 6)
#
# Includes build rules for um


CC = gcc # The compiler being used

IFLAGS = -I/comp/40/build/include -I/usr/sup/cii40/include/cii

CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

LDLIBS = -lbitpack -larith40 -l40locality -lnetpbm -lcii40 -lm -lrt -lum-dis -lcii

INCLUDES = $(shell echo *.h)

############### Rules ###############

all: um

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

um: um.o um_load_program.o um_segment.o um_execute.o um_memory.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f um *.o
