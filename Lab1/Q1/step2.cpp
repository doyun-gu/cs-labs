 #include <iostream>
 #include <thread>
 #include <future>
 #include <chrono>

  /**
  * Step 2 - Passing Arguments to Threads
  * Write a program that launches a thread running a function print_numbers(int n) which prints the numbers 1 to n.
  * Pass the value 10 as the argument when creating the thread, and wait for it to complete using .join().
  */

void print_numbers (int n) {
    for (int i = 1; i <= n; ++i) {
        std::cout << i << "";
        std::cout << "\n";
    }
}

int main() {
    std::thread t (print_numbers, 10);
    t.join();
}