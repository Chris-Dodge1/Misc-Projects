// Pi estimating program written in C, with mutex lock for enforcing mutual exclusion
// With local sum calculation for better performance
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variables
double sum = 0.0;   // Final sum for pi
int n;              // Number of terms to add
int thread_count;   // Number of threads the program will run with
pthread_mutex_t mutex;  // Mutex lock

// Function prototype; to be called by threads
void* thread_sum(void* rank);

// Main function
int main(int argc, char* argv[]) {
    // Thread handles
    pthread_t* thread_handles;

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

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

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

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
    // Add terms to a local sum
    double my_sum = 0;
    for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
        my_sum += factor / (2 * i + 1);
    }

    // Acquire lock before entering critical section
    pthread_mutex_lock(&mutex);

    // Critical section
    sum += my_sum;

    // Release the lock after exiting critical section
    pthread_mutex_unlock(&mutex);

    return NULL;
}