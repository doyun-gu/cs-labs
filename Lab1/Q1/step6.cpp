 #include <iostream>
 #include <thread>
 #include <future>
 #include <chrono>

/**
 * Step 6 - Using std::this_thread::yeild
 * Write a program that creates two threads doing CPU-bound loops (counting from 1 to 10 million).
 * In one thread, occasionally call std::this_thread::yield() inside the loop; in the other, do not.
 * Compare how this changes the behaviour or runtime when you observe CPU usage.
 */

 volatile std::uint64_t sink = 0;
 
 void spin_no_yield() {
    for (std::uint64_t i = 0; i < 50'000'000ULL; ++i) {
        sink += i & 1;
    }
 }

 void spin_with_yield() {
    for (std::uint64_t i = 0; i < 50'000'000ULL; ++i) {
        sink += i & 1;
        
        if ((i % 10000ULL) == 0) {
            std::this_thread::yield();
        }
    }
 }

 int main () {
    std::thread a(spin_no_yield);
    std::thread b(spin_with_yield);
    a.join(); b.join();
    std::cout << "done, sink = "<<sink<<"\n";
 }