#include <sys/syscall.h>
#include <unistd.h>

#define __NR_GEN_RAND_NUM 548
#define __NR_COUNT_PRIME 549

int gen_rand_num(int *arr, int max){
    int ret;

    if (arr == (void *)0)
	return -1;
    if (max <= 0)
	return -1;

    ret = syscall(__NR_GEN_RAND_NUM, arr, max);

    return ret;
}

int count_prime(int *arr, int max){
    int num_count;
    if (arr == (void *)0)
	return -1;

    num_count = syscall(__NR_COUNT_PRIME, arr, max);

    return num_count;
}
