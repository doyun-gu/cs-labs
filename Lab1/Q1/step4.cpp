 #include <iostream>
 #include <thread>
 #include <future>
 #include <chrono>

 /**
 * Step 4 - Multiple Concurrent Threads
 * Create three threads (A, B, C), each running a function that prints its thread name five times with a short delay between prints.
 * Observe how their outputs interleave on the console.
 * Ensure all threads are joined before the program exits.
 */

 void say (const char* name){
    using namespace std::chrono_literals;

    for (int i = 0; i < 5; ++i) {
        std::cout << name << "("<< std::this_thread::get_id()<<")\n";
        std::this_thread::sleep_for(120ms);
    }
 }

 int main() {
    std::thread A (say, "A"), B(say, "B"), C(say, "C");

    A.join(); 
    B.join(); 
    C.join();
 }