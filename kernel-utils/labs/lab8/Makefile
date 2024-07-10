# Lab 8 Makefile

CC = gcc
OBJS = test_syscall.o syscall_example.o gettimeofday.o

all: test_syscall syscall_example gettimeofday

test_syscall: test_syscall.o
	$(CC) -o $@ $<

syscall_example: syscall_example.o
	$(CC) -o $@ $<

gettimeofday: gettimeofday.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c $<

.PHONY: clean
clean:
	rm -rf *.o
	rm -f test_syscall syscall_example gettimeofday
