// Hello world program using OpenMP
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Thread function
void Hello(void);

// Main function
int main(int argc, char *argv[]) {
    // Get thread count from the command line
    int thread_count = strtol(argv[1], NULL, 10);

    // Parallel section
# pragma omp parallel num_threads(thread_count)
    {
        Hello();
    }

    return 0;
}

// Function implementation
void Hello(void) {

    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    printf("Hello world from %d of %d\n", my_rank, thread_count);

}