// Pi estimating program written in C, serial (non-parallel) implementation
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variables
double sum = 0.0;   // Final sum for pi
int n;              // Number of terms to add
int thread_count;   // Number of threads the program will run with

// Function prototype; to be called by threads
void* thread_sum(void* rank);

// Main function
int main(int argc, char* argv[]) {
    // Thread handles
    pthread_t* thread_handles;

    // Get number of terms to add from the command line
    thread_count = strtol(argv[1], NULL, 10);
    n = strtol(argv[2], NULL, 10);

    // Allocate memory for th thread handles
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    // Create threads, and have each call the hello function
    long thread;
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, thread_sum, (void*)thread);
    }

    // Main thread waits for child threads to join back
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    // Deallocate memory used for the thread handles
    free(thread_handles);

    // Print the estimated value of pi
    double pi = 4.0 * sum;
    printf("The estimated value of pi is %f.\n", pi);

    return 0;
}

// Thread function
void* thread_sum(void* rank) {
    long my_rank = (long) rank;
    double factor;
    long long i;
    long long my_n = n / thread_count;
    long long my_first_i = my_n * my_rank;
    long long my_last_i = my_first_i + my_n;

    // Make sure the sign of the first term is correct
    if (my_first_i % 2 == 0) factor = 1.0;
    else factor = -1.0;

    // Add my_n consecutive terms, from my_first_i to my_last_i
    for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
        sum += factor / (2 * i + 1);
    }

    return NULL;
}