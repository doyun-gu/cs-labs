/**
 * Read about the problem here: https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/
 * The basic program is given below. Try it out - it shoud deadlock
 * Make "thrd-print" thread safe. It will still deadlock, but at least the output is nice.
 * Think about how you might solve the problem by preventing the deadlock. An example solution is shown in Error! Reference source not found. But there are many others.
 * How long does it take for each type of solution you implement (the restaurant wants its diners to leave quickly!)?
 */

 /**
 * This file contains a solution to the Dining Philosophers problem
 * by implementing an "asymmetric" or "resource hierarchy" solution.
 *
 * All philosophers (0-3) pick up LEFT then RIGHT.
 * The last philosopher (4) picks up RIGHT then LEFT.
 *
 * This breaks the circular wait condition and prevents deadlock.
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <chrono>

// Mutex for thread-safe printing
std::mutex print_mutex;

void thrd_print(const std::string &str) {
    // Thread safe print function
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << str;
}

const int NUM_PHILOSOPHERS = 5;

// One mutex for each chopstick. One chopstick between each philosopher
std::mutex mtxCS[NUM_PHILOSOPHERS];

void philosopher(int philosopher_id) {
    int myLeftChopstick = philosopher_id;
    int myRightChopstick = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < 3; ++i) {
        thrd_print("Philosopher " + std::to_string(philosopher_id) + " is thinking.\n");
        // Add a small random think time
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + (rand() % 200)));

        // ASYMMETRIC SOLUTION: The last philosopher picks up in reverse order
        if (philosopher_id == NUM_PHILOSOPHERS - 1) {
            // LAST PHILOSOPHER: Pick up RIGHT, then LEFT
            thrd_print("Philosopher " + std::to_string(philosopher_id) + " (asymmetric) tries to pick up 1st (RIGHT) chopstick ID " + std::to_string(myRightChopstick) + "\n");
            std::lock_guard<std::mutex> rightLock(mtxCS[myRightChopstick]);
            thrd_print("Philosopher " + std::to_string(philosopher_id) + " GOT (RIGHT) chopstick ID " + std::to_string(myRightChopstick) + "\n");

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            thrd_print("Philosopher " + std::to_string(philosopher_id) + " has cs Id " + std::to_string(myRightChopstick) + ", tries to pick up (LEFT) ID " + std::to_string(myLeftChopstick) + "\n");
            std::lock_guard<std::mutex> leftLock(mtxCS[myLeftChopstick]);
            thrd_print("Philosopher " + std::to_string(philosopher_id) + " GOT (LEFT) chopstick ID " + std::to_string(myLeftChopstick) + "\n");

        } else {
            // ALL OTHER PHILOSOPHERS: Pick up LEFT, then RIGHT
            thrd_print("Philosopher " + std::to_string(philosopher_id) + " tries to pick up 1st (LEFT) chopstick ID " + std::to_string(myLeftChopstick) + "\n");
            std::lock_guard<std::mutex> leftLock(mtxCS[myLeftChopstick]);
            thrd_print("Philosopher " + std::to_string(philosopher_id) + " GOT (LEFT) chopstick ID " + std::to_string(myLeftChopstick) + "\n");

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            thrd_print("Philosopher " + std::to_string(philosopher_id) + " has cs Id " + std::to_string(myLeftChopstick) + ", tries to pick up (RIGHT) ID " + std::to_string(myRightChopstick) + "\n");
            std::lock_guard<std::mutex> rightLock(mtxCS[myRightChopstick]);
            thrd_print("Philosopher " + std::to_string(philosopher_id) + " GOT (RIGHT) chopstick ID " + std::to_string(myRightChopstick) + "\n");
        }


        // She has two chopsticks. Eating time!
        thrd_print("Philosopher " + std::to_string(philosopher_id) + " is EATING for 3 secs!\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        thrd_print("Philosopher " + std::to_string(philosopher_id) + " is putting down chopsticks.\n");
        // Both lock_guards go out of scope here, releasing the mutexes
    }
    thrd_print("Philosopher " + std::to_string(philosopher_id) + " is FINISHED and leaving.\n");
}

int main() {
    std::thread athrdPhilosophers[NUM_PHILOSOPHERS];

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        // Corrected thread creation
        athrdPhilosophers[i] = std::thread(philosopher, i);
    }

    for (auto &p : athrdPhilosophers) {
        p.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // This line WILL be reached
    thrd_print("------------------------------------------\n");
    thrd_print("All philosophers finished eating.\n");
    thrd_print("Total execution time: " + std::to_string(diff.count()) + " s\n");

    return 0;
}
