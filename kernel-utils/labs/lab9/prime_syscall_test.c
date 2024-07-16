#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "prime_syscall.h"


// Function to print usage message
void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s -n <number>\n", prog_name);
}

int main(int argc, char **argv){
    int max = 0;
    int opt;
    int *arr;
    int res;

    struct timeval start, end;

    srand(time(NULL));
					//
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                max = atoi(optarg);
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (max <= 0) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL); // Start timing
					//
    arr = (int *)malloc(sizeof(int) * max);
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    gen_rand_num(arr, max);
    res = count_prime(arr, max);

    gettimeofday(&end, NULL); // End timing

    // Calculate the time taken in microseconds
    long processing_time = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;

    // Print the results
    printf("The number of total random numbers: %d\n", max);
    printf("The number of prime numbers: %d\n", res);
    printf("Processing time: %ld us\n", processing_time);

    free(arr); // Free allocated memory

    return 0;
}

