#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

// Function to get the nearest square root of a number
int get_nearest_sqrt(int num){
    int iter = 1;
    while (iter * iter < num){
        iter++;
    }
    return iter;
}

// Function to check if a number is prime
int check_if_prime(int num){
    if (num < 2) return 0; // Numbers less than 2 are not prime
    int sqrt_of_num = get_nearest_sqrt(num);
    for (int iter = 2; iter <= sqrt_of_num; iter++){
        if (num % iter == 0)
            return 0;
    }
    return 1;
}

// Function to generate random numbers
void generate_random_number(int *arr, int arr_size, int max){
    for (int i = 0; i < arr_size; i++){
        arr[i] = rand() % max;
    }
}

// Function to print usage message
void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s -n <number>\n", prog_name);
}

int main(int argc, char **argv){
    int max = 0;
    int opt;
    int *arr;

    struct timeval start, end;

    srand(time(NULL));
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

    arr = (int *)malloc(sizeof(int) * max);
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    generate_random_number(arr, max, max);

    gettimeofday(&start, NULL); // Start timing

    int prime_count = 0;
    for (int i = 0; i < max; i++){
        if (check_if_prime(arr[i])) {
            prime_count++;
        }
    }

    gettimeofday(&end, NULL); // End timing

    // Calculate the time taken in microseconds
    long processing_time = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;

    // Print the results
    printf("The number of total random numbers: %d\n", max);
    printf("The number of prime numbers: %d\n", prime_count);
    printf("Processing time: %ld us\n", processing_time);

    free(arr); // Free allocated memory

    return 0;
}

