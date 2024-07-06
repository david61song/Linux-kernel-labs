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

	/* TODO: If you need sychronization, add ioctl for locking */
	pid = fork();

	if (pid > 0) {
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);

		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);

		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);
		ioctl(fd, MY_IOCTL_PRINT0, NULL);
		ioctl(fd, MY_IOCTL_RESET, NULL);

		/* expected answer is 15 */


		ioctl(fd, MY_IOCTL_INC_ALL, NULL);
		ioctl(fd, MY_IOCTL_INC_ALL, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);
		ioctl(fd, MY_IOCTL_PRINT1, NULL);
		ioctl(fd, MY_IOCTL_RESET, NULL);

		/* expected answer is 10 */

	} else if (pid == 0) {
		ioctl(fd, MY_IOCTL_INT, NULL);
		// /* expected answer is -15 */
		ioctl(fd, MY_IOCTL_RESET, NULL);

		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_INC_0, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);

		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_INC_1, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);

		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_INC_2, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);

		ioctl(fd, MY_IOCTL_INC_3, NULL);
		ioctl(fd, MY_IOCTL_INC_3, NULL);
		ioctl(fd, MY_IOCTL_INC_3, NULL);
		ioctl(fd, MY_IOCTL_INC_3, NULL);
		ioctl(fd, MY_IOCTL_INC_3, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);

		ioctl(fd, MY_IOCTL_INC_4, NULL);
		ioctl(fd, MY_IOCTL_INC_4, NULL);
		ioctl(fd, MY_IOCTL_INC_4, NULL);
		ioctl(fd, MY_IOCTL_INC_4, NULL);
		ioctl(fd, MY_IOCTL_INC_4, NULL);
		ioctl(fd, MY_IOCTL_SUM, NULL);
		ioctl(fd, MY_IOCTL_PRINT2, NULL);
		ioctl(fd, MY_IOCTL_RESET, NULL);

		/* expected answer is 25 */
		return 0;
	}
	close(fd);

	return 0;
}
