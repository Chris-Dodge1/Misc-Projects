// Pi estimating program written in C, parallelized using OpenMP
#include <stdio.h>
#include <stdlib.h>

// Global variable, denotes how many terms there are, and the sum for pi
double sum = 0.0;
int n;

// Main function
int main(int argc, char* argv[]) {

// Get number of terms to add from the command line
n = strtol(argv[1], NULL, 10);

// Get thread count from the command line
int thread_count = strtol(argv[2], NULL, 10);

// Add terms in the summaiton for the estimated value of pi
double factor = 1.0;

# pragma omp parallel for num_threads(thread_count) private(factor) reduction(+: sum)
for (int i = 0; i < n; i++) {
    if (i % 2 == 0) factor = 1.0;
    else factor = -1.0;
    sum += factor / (2 * i + 1);
}

double pi = 4.0 * sum;

printf("The estimated value of pi is %f.\n", pi);

return 0;
}