#include <iostream>
#include <thread>
#include <vector>
#include <random>       // Provides std::uniform_real_distribution and std:mt19937
#include <chrono>
#include <mutex>
#include <sstream>

// This mutex protects std::cout
std::mutex cout_mutex;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << msg << std::endl;
}

class RandomTwister {
    public:
        // Constructor to initialise the random generator with a specific range
        // FIX: Initialize both members in the initializer list.
        //      The engine is seeded using std::random_device for a non-deterministic seed.
        RandomTwister (float min, float max)
            : distribution(min, max), engine(std::random_device{}()) {}

        // Returns a random float within the specified range
        float generate() {
            // CRITICAL FIX: Add a lock_guard to make this method thread-safe.
            // This prevents multiple threads from accessing the 'engine' at the same time,
            // which would cause a data race.
            std::lock_guard<std::mutex> lock(gen_mutex_);
            return distribution(engine);
        }

    private:
        // FIX: Add a mutex for thread-safety
        std::mutex gen_mutex_;

        // FIX: Correctly declare members.
        std::mt19937 engine;
        std::uniform_real_distribution<float> distribution;
};

void generateAndPrintRandom(RandomTwister& r) {
    std::ostringstream oss;
    
    // FIX: Add a space for readability and remove extra semicolon
    oss << "Random Float: " << r.generate();
    
    std::string msg = oss.str(); // Convert to a string
    safe_print(msg);
}

int main () {
    RandomTwister generator (1.0f, 5.0f); // Random floats between 1.0 and 5.0 
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.push_back(std::thread(generateAndPrintRandom, std::ref(generator)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
