// Even-odd transposition sort
// Data structures and printing
#include <iostream>
#include <vector>
#include <random>       // For RNG-related features
#include <algorithm>    // For swap
#include <omp.h>
#include <chrono>

// Function prototypes
std::vector<int> GenerateRandomIntegers(int size);
void OddEvenSort(std::vector<int>& arr, int thread_count);

// Main function
int main(int argc, char* argv[]) {

    // Parse thread count as a command-line argument
    int array_size = strtol(argv[1], NULL, 10);

    // Parse thread count as a command-line argument
    int thread_count = strtol(argv[2], NULL, 10);

    // TODO 1: Generate an array of random integers.
    std::vector<int> random_ints = GenerateRandomIntegers(array_size);

    // TODO 2: Print the integers before sorting.
    // If the array is too large, print the first few numbers
    // (and optionally the last few numbers).
    for (int i = 0; i < 20; i++) {
        std::cout << random_ints[i] << " ";
    }
    std::cout << std::endl;

    // TODO 3.1: get the starting time
    auto start = std::chrono::steady_clock::now();

    // TODO 3.2: Timekeeping step 2: call the sorting algortihm here.
    // For consistency, ONLY call the code for the sorting algortithm,
    // Any code for generating the array should run before step 1.
    // Additionally, any code for printing the array (before and after)
    // Should run before step 1 and after step 3.
    OddEvenSort(random_ints, thread_count);

    // Timekeeping step 3.3: get the ending time, then print elapsed time
    auto stop = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Executed time in milliseconds: " <<elapsed_time.count() << std::endl;

    // TODO 4: Print the integers after sorting.
    // IF the array is too large, print the first few numbers
    // (and optionally the last few numbers).
    for (int i = 0; i < 20; i++) {
        std::cout << random_ints[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

// Function; makes a random vector of integers
// Uses <random> insteaf of rand() for better randomness
// May not be the best implementation, but we're more concerned
// about the performance of a sorting algortihm, not the RNG.
std::vector<int> GenerateRandomIntegers(int size) {

    // Create a random device to seed the random-number engine
    std::random_device rd;

    // Random-number engine
    std::mt19937 engine(rd());

    // Define a uniform distribution
    // Numbers are from 1 (inclusive) to 1000000 (inclusive)
    std::uniform_int_distribution<int> dist(1, 1000000);

    // Reserve space for the vector
    std::vector<int> random_ints;
    random_ints.reserve(size);

    // Generate integers using a for loop
    for (int i = 0; i < size; i++) {
        int rand_num = dist(engine);
        random_ints.push_back(rand_num);
    }

    return random_ints;
}

// Function; odd-even sort (or brick sort)
// Pass in a vector to start sorting
void OddEvenSort(std::vector<int>& arr, int thread_count) {

    // Size of the array
    int n = arr.size();

    // Implementation uses a doubly-nested for loop
    // Outer loop controls the phase
    // Within that is two for loops; which one executes depends on the phase
    # pragma omp parallel num_threads(thread_count) shared(n, arr, thread_count) \
        default(none)
    for (int p = 0; p < n; p++) {
        if (p % 2 == 0) {
            // Even phase
            // Odd-indexed elements are swapped with the element to the
            // left, if out-of-order.
            # pragma omp for
            for (int i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                        std::swap(arr[i], arr[i + 1]);
                    }
                }
            } else {
                // Odd phase
                // Odd-indexed elements are swapped with the element to the
                // right, if out-of-order.
                # pragma omp for
                for (int i = 0; i < n - 1; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        std::swap(arr[i], arr[i + 1]);
                    }
                }
            }

            # pragma omp barrier
        }

    std::cout << "Sorting complete.\n\n";
}