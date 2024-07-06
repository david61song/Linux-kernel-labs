CC = gcc
OBJS = test_syscall.o
TARGET = test_syscall

all:
	make test_syscall

test_syscall: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	@rm -f $(OBJS)

test_syscall.o: test_syscall.c
	$(CC) -c test_syscall.c


.PHONY: clean
clean:
	rm -f $(TARGET)
