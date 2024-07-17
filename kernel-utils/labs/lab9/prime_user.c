#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

// Function to get the nearest square root of a number
static int get_nearest_sqrt(int num){
    int iter = 1;
    while (iter * iter < num){
        iter++;
    }
    return iter;
}

// Function to check if a number is prime
static int check_if_prime(int num){
    if (num < 2) 
        return 0; // Numbers less than 2 are not prime
    int sqrt_of_num = get_nearest_sqrt(num);
    for (int iter = 2; iter <= sqrt_of_num; iter++){
        if (num % iter == 0)
            return 0;
    }
    return 1;
}

// Function to generate random numbers and count prime

static int count_prime(int max){
    int prime = 0;
    for(int i = 0 ; i < max ; i ++){
        if (check_if_prime(rand() % max))
            prime ++;
    }
    return prime;
}



// Function to print usage message
static void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s -n <number>\n", prog_name);
}

int main(int argc, char **argv){
    int max = 0;
    int opt;
    int *arr;
    struct timeval start, end, result;
    long long processing_time;
    int prime_count = 0;

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
    srand(time(NULL));
    /*****------- Start timing -------*****/
    gettimeofday(&start, NULL); 
    prime_count = count_prime(max);
    gettimeofday(&end, NULL); 
    /*****------- End timing -------*****/

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

