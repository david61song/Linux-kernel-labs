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
    int prime_count;
    struct timeval start, end, result;
    long long processing_time;
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

    gettimeofday(&start, NULL); 
    /*****------- Start timing -------*****/
    arr = (int *)malloc(sizeof(int) * max);
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    gen_rand_num(arr, max);
    prime_count = count_prime(arr, max);
    gettimeofday(&end, NULL);
    /*****------- end timing -------*****/

    // Calculate the time taken in microseconds
    timersub(&end, &start, &result);
    processing_time = result.tv_sec * 1000000 + result.tv_usec;
    // Print the results
    printf("The number of total random numbers: %d\n", max);
    printf("The number of prime numbers: %d\n", prime_count);
    printf("Processing time: %lld us\n", processing_time);

    free(arr); // Free allocated memory

    return 0;
}

