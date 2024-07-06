/*
 * SCE394 - Memory Mapping Lab (#14)
 *
 * memory mapping between user-space and kernel-space
 *
 * test case
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NPAGES		16
#define MMAP_DEV	"/dev/mymmap"

/* PAGE_SIZE = getpagesize() */

int test_write_read(int fd, unsigned char *mmap_addr)
{
	int i;
	printf("\nWrite/Read test ...\n");

	/* TODO: write to device mmap'ed address */

	/* TODO: call mymap_read() */

	return 0;
}

int main(int argc, const char **argv)
{
	int fd;
	unsigned char *addr;
	int len = NPAGES * getpagesize();
	int i;

	fd = open(MMAP_DEV, O_RDWR | O_SYNC);
	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	/* TODO: call mmap() system call with R/W permission to address */

	/* TODO: check the values by module init */

	/* TODO: call test_write_read() */

	close(fd);

	return 0;
}
