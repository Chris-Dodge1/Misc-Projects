// Trapezodal rule program using OpenMP
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Thread function
void Trap(double a, double b, int n, double* global_result_p);
double f(double x);

// Main function
int main(int argc, char *argv[]) {

    double global_result = 0.0; // Global results; total area under the curve
    double a, b;        // Leftmost and rightmost endpoints
    int n;              // Number of trapezoids
    int thread_count;

    // Get thread count from command line
    thread_count = strtol(argv[1], NULL, 10);

    // Prompt user to enter a, b, and n
    printf("Enter endpoints a and b, and the number of trapezoids n\n");
    scanf("%lf %lf %d", &a, &b, &n);

    // Parallel section; each thread calls Trap
    // and each thread calculated its own portion of trapezoids
    # pragma omp parallel num_threads(thread_count)
    {
        Trap(a, b, n, &global_result);
    }

    // Print the estimated area under the curve
    printf("With %d trapezoids, the area between %f and %f is %f\n",
           n, a, b, global_result);

    return 0;
}

// Function implementation for Trap()
void Trap(double a, double b, int n, double* global_result_p) {
    // h is the width of a trapezoid 
    // x is one of the trapezoid's heights
    // my_result is the thread's local result
    // local_a and local_b are the thread's local endpoints
    double h, x, my_result;
    double local_a, local_b;

    // i is used for the for loop
    // local_n is the thread's local number of trapezoids
    int i, local_n;

    // Rank and thread count
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    // Calculate the width of a single trapezoid
    h = (b - a) / n;

    // Calculate local trapezoid count and local endpoints
    local_n = n / thread_count;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;

    // Initialize my_results
    my_result = (f(local_a) + f(local_b)) / 2.0;

    // Add all other trapezoids
    for (i = 1; i <= local_n - 1; i++) {
        // Calculate the height of a trapezoid
        x = local_a + i * h;
        my_result += f(x);
    }
    my_result = my_result * h;

    // Add local results to global result
    // this is a critical section
    # pragma omp critical
    {
    *global_result_p += my_result;
    }
}

// Function implementation for f(x); its just f(x) = x^2
double f(double x) {
    return x * x;
}