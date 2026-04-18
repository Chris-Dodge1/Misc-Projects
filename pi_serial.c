// Pi estimating program written in C, serial (non-parallel) implementation
#include <stdio.h>
#include <stdlib.h>

// Global variable, denotes how many terms there are, and the sum for pi
double sum = 0.0;
int n;

// Main function
int main(int argc, char* argv[]) {

// Get number of terms to add from the command line
n = strtol(argv[1], NULL, 10);

// Add terms in the summaiton for the estimated value of pi
double factor = 1.0;

for (int i = 0; i < n; i++, factor = -factor) {
    sum += factor / (2 * i + 1);
}

double pi = 4.0 * sum;

printf("The estimated value of pi is %f.\n", pi);

return 0;
}