#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NPAGES          16
#define MMAP_DEV        "/dev/mymmap"
#define TEST_STR_SIZE   16 * 4096

int test_write_read(int fd, unsigned char *mmap_addr, size_t mmap_size)
{
    int pgsize = getpagesize();
    printf("\n>>>>Write/Read test..\n");
    char *test_string = "Welcome to mmap() world!";
    char read_string[TEST_STR_SIZE];

    /* write to device mmap'ed address */
	/* write each page */
    for (int i = 0; i < mmap_size; i += pgsize) {
        memcpy(mmap_addr + i, test_string, strlen(test_string));
    }

    /* call mymap_read() */
    ssize_t bytes_read = read(fd, read_string, TEST_STR_SIZE - 1);
    if (bytes_read < 0) {
        perror("read");
        return -1;
    }

    return 0;
}

int main(int argc, const char **argv)
{
    int fd;
    unsigned char *addr;
    size_t len = NPAGES * getpagesize();

    fd = open(MMAP_DEV, O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Device successfully opened. start test.. \n");

    /* call mmap() system call with R/W permission to address */
    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* call test_write_read() */
    if (test_write_read(fd, addr, len) < 0) {
        printf("Test write/read failed\n");
    }

    /* Unmap the memory */
    if (munmap(addr, len) < 0) {
        perror("munmap");
    }

    close(fd);
    return 0;
}