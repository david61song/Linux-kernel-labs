#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#define NR_my_syscall 548

int main(void) {
	int ret = 0;
	ret = syscall(NR_my_syscall);
	if (ret != 0) {
		fprintf(stderr, "ERR[%d]: syscall %d is failed\n", ret, NR_my_syscall);
		return -1;
	}
	return 0;
}
