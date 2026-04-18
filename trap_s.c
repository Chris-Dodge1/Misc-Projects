// Trapezodal rule program using OpenMP, fourth version
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Thread function
double Trap(double a, double b, int n, int thread_count);
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

    // Call the trap function
    global_result += Trap(a, b, n, thread_count);

    // Print the estimated area under the curve
    printf("With %d trapezoids, the area between %f and %f is %f\n",
           n, a, b, global_result);

    return 0;
}

// Function implementation for Trap()
double Trap(double a, double b, int n, int thread_count) {
    // h is the width of a trapezoid 
    // x is one of the trapezoid's heights
    // approx is the thread's local result
    double h, x, approx;

    // i is used for the for loop
    int i;

    // Calculate the width of a single trapezoid
    h = (b - a) / n;

    // Initialize approx
    approx = (f(a) + f(b)) / 2.0;

    // Add all other trapezoids
    # pragma omp parallel for num_threads(thread_count) reduction(+: approx) private(x)
    for (i = 1; i <= n - 1; i++) {
        int my_rank = omp_get_thread_num();
        // Calculate the height of a trapezoid
        x = a + i * h;
        printf("Thread %d of %d has calculated x: %f\n", my_rank, thread_count, x);
        approx += f(x);
    }
    approx = approx * h;

    return approx;
}

// Function implementation for f(x); its just f(x) = x^2
double f(double x) {
    return x * x;
}