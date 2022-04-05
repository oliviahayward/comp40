############## Variables ###############

CC = gcc # The compiler being used

IFLAGS = -I/comp/40/build/include -I/usr/sup/cii40/include/cii

CFLAGS = -g -O2 -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

LDLIBS = -lbitpack -lcii40-O2 -lnetpbm -lm -lrt

INCLUDES = $(shell echo *.h)

############### Rules ###############

all: um

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

um: um.o um_load_program.o um_segment.o um_execute.o um_memory.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f um *.o
