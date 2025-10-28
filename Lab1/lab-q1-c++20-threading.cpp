 #include <iostream>
 #include <thread>
 #include <future>
 #include <chrono>

/**
 * Step 1 - Creating and Joining a Thread
 * Write a C++ program that creates one additional thread which prints "Hello from the worker thread!", while the main thread prints "Hello from main!".
 * Use std::thread and make sure both threads finish correctly without crashing by using .join().
 * Print the therad IDs using std::this_thread::get_id().
 */

//  void worker() {
//     std::cout << "Hello from the worker thread! id =" << std::this_thread::get_id() << "\n";
//  }

//  int main () {
//     std::cout << "Hello from main! id=" << std::this_thread::get_id() << "\n";
//     std::thread t(worker);
//     t.join();
//  }

 /**
  * Step 2 - Passing Arguments to Threads
  * Write a program that launches a thread running a function print_numbers(int n) which prints the numbers 1 to n.
  * Pass the value 10 as the argument when creating the thread, and wait for it to complete using .join().
  */

// void print_numbers (int n) {
//     for (int i = 1; i <= n; ++i) {
//         std::cout << i << "";
//         std::cout << "\n";
//     }
// }

// int main() {
//     std::thread t (print_numbers, 10);
//     t.join();
// }

/**
 * Step 3 - Use std::this_thread::sleep_for
 * Modify your Lab 2 program so that each number if printed 200m s apart using std::this_thread::sleep_for (std::chrono::miliseconds(200));
 * Oberserve how this affects the timing of the output.
 */

// void print_numbers_slow (int n) {
//     using namespace std::chrono_literals;

//     for (int i = 1; i <= n; ++i) {
//         std::cout << i << "" << std::flush;
//         std::this_thread::sleep_for(200ms);
//     }

//     std::cout<<"\n";
// }

// int main() {
//     std::thread t(print_numbers_slow, 10);
//     t.join();
// }

/**
 * Step 4 - Multiple Concurrent Threads
 * Create three threads (A, B, C), each running a function that prints its thread name five times with a short delay between prints.
 * Observe how their outputs interleave on the console.
 * Ensure all threads are joined before the program exits.
 */

//  void say (const char* name){
//     using namespace std::chrono_literals;

//     for (int i = 0; i < 5; ++i) {
//         std::cout << name << "("<< std::this_thread::get_id()<<")\n";
//         std::this_thread::sleep_for(120ms);
//     }
//  }

//  int main() {
//     std::thread A (say, "A"), B(say, "B"), C(say, "C");

//     A.join(); 
//     B.join(); 
//     C.join();
//  }

 /**
  * Step 5 - Thread Synchronisation with std::mutex
  * Write a program where two threads both increment a shared global counter variable 1000 times.
  * First run it without any locking -- what happens?
  * Then add a std::mutex to protect the counter using lock_guard<std::mutex> so the result is correct.
  */

// int counter = 0;
// std::mutex m;

// void add_1000() {
//     for (int i = 0; i < 1000; ++i) {
//         std::lock_guard <std::mutex> lg(m);
//         ++counter;
//     }
// }

// int main() {
//     std::thread t1(add_1000), t2(add_1000);
//     t1.join(); t2.join();
//     std::cout << "counter =" << "(expected 2000)\n";
// }

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

