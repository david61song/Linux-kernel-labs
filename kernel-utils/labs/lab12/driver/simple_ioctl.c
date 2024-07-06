#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>

#include "driver.h"

int main(int argc, char **argv) {
	int fd;
	pid_t pid;

	fd = open("/dev/mycdev", O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open error\n");
		return fd;
	}

	printf("This is simple ioctl call for irq handler\n");
	ioctl(fd, MY_IOCTL_INT, NULL);

	close(fd);

	return 0;
}
