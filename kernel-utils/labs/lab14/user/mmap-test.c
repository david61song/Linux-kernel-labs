/*
 * Memory Mapping Lab (#14)
 *
 * memory mapping between user-space and kernel-space
 *
 * test case program
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

#define NPAGES			16
#define MMAP_DEV		"/dev/mymmap"
#define TEST_STR_SIZE	128

/* PAGE_SIZE = getpagesize() */

int test_write_read(int fd, unsigned char *mmap_addr)
{
	int i;
	printf("\n>>>>Write/Read test..\n");
	char *test_string = "Welcome to mmap() world!";
	char read_string[TEST_STR_SIZE];

	/* write to device mmap'ed address */
	memcpy(mmap_addr, test_string, strlen(test_string));

	/* call mymap_read() */

	read(fd, read_string, TEST_STR_SIZE - 1);

	/*gurantee Null-terminated string*/
	read_string[TEST_STR_SIZE - 1] = '\0';

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

	printf("Device successfully opened. start test.. \n");

	/* call mmap() system call with R/W permission to address */

	addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	/* check the values by module init */

	if (addr == NULL){
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	/* call test_write_read() */

	test_write_read(fd, addr);

	close(fd);

	return 0;
}
