/*
* Write a simple C++ program that implements a cyclic execution with a monor cycle of 100 ms.

* Your program should schedule and repeatedly execute three periodic tasks within each cycle:
* Task A: Runs every 100 ms and prints "Reading sensor..."
* Task B: Runs every 200 ms and prints "Updating control..."
* Task C: Runs every 500 ms and prints "Transmitting data..."

* Use std::chrono and std::this_thread::sleep_until() to keep the timing accurate.
* The program should run for about 5 seconds, maintain fixed timing, and print the task outputs in their correct periodic order.
*/


#include <chrono>     // Used for time-related functions like sleep_for
#include <iostream>   // Used for console input/output (like cout)
#include <thread>     // The main library for creating and managing threads

using namespace std;

// This is the function that the thread will execute.
void F_code() {
    cout << "F_code started\n";
    
    // Pause this thread for 3 seconds to simulate a long-running task.
    this_thread::sleep_for(chrono::seconds(3)); 
    
    // Print a message when the task is done, including the unique ID of this running thread.
    // this_thread::get_id() gets the ID of the *currently executing* thread.
    cout << "F_code finished with thread ID " << this_thread::get_id() << endl;
}

// This function creates a new thread and returns the std::thread object that manages it.
thread my_create_thread() {
    // A new thread 't' is created. As soon as this line is executed,
    // the F_code function starts running concurrently on a separate thread.
    thread t(F_code);
    
    // This line prints the ID of the *thread running the my_create_thread function* (likely the main thread),
    // NOT the ID of the new thread 't' that was just created.
    cout << "my_create_thread: The creator thread's ID is " << this_thread::get_id() << endl;
    
    // The thread object 't' is returned. Because a std::thread cannot be copied,
    // it is automatically "moved" back to whoever called the function.
    return t; 
}

int main () {
    // Create two uninitialized thread objects. They are not associated with any running thread yet.
    // Calling get_id() on them will show a default value indicating "not a thread".
    thread t1, t2;
    cout << "main: At creation, t1 ID is " << t1.get_id() << ", t2 ID is " << t2.get_id() << endl;

    // Call my_create_thread() which starts F_code running on a new thread.
    // The returned thread object is moved into t1. Now, t1 "owns" and manages the new thread.
    t1 = my_create_thread(); 
    cout << "main: Before move, t1's associated thread ID is " << t1.get_id() << endl;

    // Here is the key step: std::move(t1) transfers ownership of the running thread from t1 to t2.
    // After this line:
    // - t2 now owns the thread that is executing F_code.
    // - t1 becomes an empty, uninitialized thread object again (as if it were just created).
    t2 = std::move(t1);
    cout << "main: After move, t1's ID is now " << t1.get_id() << endl; // Will be "not a thread"
    cout << "main: After move, t2's ID is now " << t2.get_id() << endl; // Will show the ID of the running thread

    // The main thread must wait for the thread managed by t2 to finish its execution.
    // This is called "joining". If you don't join a thread, the program might terminate unexpectedly.
    t2.join();
    
    // This line will only be printed after the F_code function has completed.
    cout << "Main finished\n";

    return 0;
}