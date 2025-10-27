/**
 * Demonstrate that the std::mt19937 is not thread-safe by running the pgoramme multiple times. If it were thread-safe, the programme would wlays produce the same result because the random number generator's seed is fixed.
 * Add Timing code to find out how long the program takes to run.
 * Once you've done that, look at the follow-on tasks below:
 */

#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>           // For timing
#include <functional>       // For std::ref

// Global engine (seeded for consistent single-thread runs)
std::mt19937 gen(12345); 

// Global distribution (matches your 'correct sum' of 50460531)
// This is also not thread-safe!
std::uniform_int_distribution<> dist(0, 100); 

/**
 * @brief Worker function to generate random numbers and sum them.
 * * WARNING: This function demonstrates a data race.
 * It accesses the global 'gen' and 'dist' variables concurrently
 * from multiple threads without any locks.
 */
void worker (std::atomic<long long>& totalSum, int iterations) {
    long long localSum = 0;
    
    for (int i = 0; i < iterations; ++i) {
        // !!! DATA RACE !!!
        // Unsafe concurrent access to shared 'dist' and 'gen'.
        // Both threads read/write the internal state of 'gen'
        // at the same time, corrupting the sequence.
        localSum += dist(gen);
    }

    // Safe accumulation using atomic addition
    totalSum.fetch_add(localSum, std::memory_order_relaxed);
}

int main () {
    // Must be > 1 to show a data race!
    const int NUM_THREADS = 4;
    const int NUM_ITERS_PER_THREAD = 1000000 / NUM_THREADS; // Keep total work the same

    std::atomic<long long> totalSum(0);
    std::vector<std::thread> threads;

    // --- Start Timing ---
    auto start = std::chrono::high_resolution_clock::now();

    // Launch threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        // Correct way to launch the thread:
        // Pass the function, then its arguments.
        // std::ref() is needed for references (like the atomic)
        threads.push_back(
            std::thread(worker, std::ref(totalSum), NUM_ITERS_PER_THREAD)
        );
    }

    // Wait for all threads to finish
    for (std::size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    // --- Stop Timing ---
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // Print the final result *after* all threads are joined
    std::cout << "Target sum (from 1 thread) = 50460531" << std::endl;
    std::cout << "Actual total sum from " << NUM_THREADS << " threads = " << totalSum.load() << std::endl;
    std::cout << "Time taken: " << diff.count() << " seconds" << std::endl;

    return 0;
}