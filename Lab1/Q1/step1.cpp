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

 void worker() {
    std::cout << "Hello from the worker thread! id =" << std::this_thread::get_id() << "\n";
 }

 int main () {
    std::cout << "Hello from main! id=" << std::this_thread::get_id() << "\n";
    std::thread t(worker);
    t.join();
 }