// Fibonacci numbers program
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Main function
int main(int argc, char* argv[]) {

    // Parse thread count from command line args
    int thread_count = strtol(argv[1], NULL, 10);

    // Create an array to store the first 40 Fibonacci numbers
    const int n = 40;
    int fibo[n];
    fibo[0] = fibo[1] = 1;

    // Calculate the first 40 Fibonacci numbers
    # pragma omp parallel for num_threads(thread_count)
    for (int i = 2; i < n; i++) {
        fibo[i] = fibo[i - 1] + fibo[i - 2];
    }

    // Print
    printf("The Fibonacci numbers are:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", fibo[i]);
    }

    printf("End of program reached.\n");

    return 0;

}