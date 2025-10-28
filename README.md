# Concurrent Systems Lab
## Electrical and Electronics Engineering | University of Manchester

This repository contains the lab exercises for the Concurrent Systems module, focusing on C++ threading. It covers setup for both Windows and macOS, followed by 8 progressive steps to understand and implement C++ concurrency.

## C++ Set up Requirement

### Windows

As this module uses `#include<thread>`, the default MinGW `g++` compiler (which might come with IDEs like Code::Blocks) may not support it correctly. This method uses MSYS2 to install an up-to-date toolchain that fully supports C++17 threading.

#### 1. Install MSYS2

Copy and paste the following code in **PowerShell**.

```sh
winget install MSYS2.MSYS2
```

#### 2. Run `MSYS2 UCRT64` Terminal

> [!NOTE]
> This is not your standard Command Prompt or PowerShell! YOu must run the **"MSYS2 UCRT64"** application from the Start Menu.

#### 3. Check update & toolchain at `MSYS2` Terminal
Run the following commands in the MSYS2 Terminal.

```sh
pacman -Syu        
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

>[!NOTE]
> `pacman -Syu` may ask you to close the terminal after its first run. 
> If it does, close it, re-open it, and run the command again to complete all updates before proceeding.

#### 4. Check Installation

```sh
g++ --version
```

This should output a version number (e.g., `13.2.0` or higher) and confirm it's the `mingw64` toolchain.

#### 5. Compile using thread

Use this command to compile any of the lab files. The `-pthread` flag is essential.

```sh
g++ -std=c++17 -pthread main.cpp -o main.exe
```

---

### Mac OS

#### Install Homebrew (Optional if you already have)

Open your **Terminal** and install Homebrew:

```bash
/bin/bash -c "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh](https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh))"
```

#### 2. Install GCC Toolchain

macOS comes with `clang` by default, which is an excellent compiler. However, to maintain consistency with the Windows setup and use the same GNU `g++` compiler, it's best to install the gcc suite from Homebrew.

```bash
brew install gcc
```

#### 3. Compile using thread

Homebrew installs `g++` with a version number to avoid conflicting with the system's default compiler. After installing, your command will likely be `g++-13` or `g++-14` (you can check by typing `g++-` and hitting `Tab`).

>[!NOTE]
> Replace `g++-13` with your installed version)
> To run it: `./main

---

## Threading (8 Steps)

Here are the 8 lab exercises, from basic thread creation to synchronisation and futures.

### Step 1: Create and Joining a Thread

#### Task

Write a C++ program that creates one additional thread (a "worker thread").
* The **worker thread** should print: "Hello from the worker thread!"
* The **main thread** should print: "Hello from main!"
* Both threads should also print their unique thread ID.
* Use `std::thread::join()` to ensure the `main` function waits for the worker thread to finish before exiting.

#### Key Concepts

* `#include <thread>`: The header required for all C++ threading.
* `std::thread my_thread(function_name)`: This is how you create a new thread. The constructor takes a *callable* (like a function name, lambda, etc.) and its arguments. Execution begins immediately.
* `my_thread.join()`: This "joins" the threads. The thread that calls `join()` (in this case, `main`) will pause and wait until `my_thread` has finished its execution. This is critical for preventing the `main` function from ending and terminating the entire program before the worker thread is done.
* `std::this_thread::get_id()`: A function that returns the unique `thread::id` of the thread that is currently executing it.

---

### Step 2: Passing Arguments to Threads

#### Task

Write a program that launches a thread running a function `print_numbers(int n)` which prints numbers from 1 to `n`. Pass the value `10` as the argument `n` during thread creation. The main thread must wait for the worker thread to complete using `join()`.

#### Key Concepts

* `std::thread(func, arg1, arg2, ...)`: Arguments for the thread's function are passed as additional arguments to the `std::thread` constructor.
* **Argument Copying**: Arguments are **copied** into the new thread's storage by default. To pass by reference, you must wrap the argument in `std::ref()` (e.g., `std::ref(my_variable)`).

---

### Step 3: Use `std::this_thread::sleep_for`

#### Task

Modify the program from Step 2. Inside the `print_numbers` function, make the thread pause for 200 milliseconds *after* printing each number. Observe how this affects the timing of the output.

#### Key Concepts

* `#include <chrono>`: The header required for C++ time and duration utilities.
* `std::this_thread::sleep_for(duration)`: Pauses the *current* thread for a specified amount of time.
* `std::chrono::milliseconds(200)`: Creates a `duration` object representing 200 milliseconds. Other units like `seconds`, `microseconds`, etc., are also available.

---

### Step 4: Multiple Concurrent Threads

#### Task

Create three threads (A, B, and C). Each thread should run a function that prints its own name (e.g., "A") five times, with a short delay in the loop. Observe how their outputs interleave on the console. Ensure all threads are joined before the program exits.

#### Key Concepts

* **Concurrency vs. Parallelism**: Concurrency is about tasks *making progress* at overlapping times (interleaving). Parallelism is about tasks *running* at the exact same time (requires multiple CPU cores).
* **Interleaving Output**: You will see the outputs (A, B, C) are mixed up. This is because the Operating System's scheduler decides which thread to run at any moment, and this scheduling is non-deterministic.
* **Managing Threads**: Using a `std::vector<std::thread>` is a common pattern to create and later join multiple threads.

---

### Step 5: Thread Synchronisation with `std::mutex`

#### Task

Write a program where two threads both increment a shared global variable 1,000,000 times each.
1.  First, run it *without* any locking to observe the **data race** (the final result will be incorrect and less than 2,000,000).
2.  Second, add a `std::mutex` and use `std::lock_guard` to protect the shared variable, ensuring the final result is correct.

#### Key Concepts

* **Data Race**: Occurs when two or more threads access the same memory location concurrently, and at least one of the accesses is a write. The `++` operation (read-modify-write) is not **atomic**.
* `#include <mutex>`: The header required for mutexes.
* `std::mutex`: A "Mutual Exclusion" object. It provides `lock()` and `unlock()` methods. Only one thread can hold a lock on a mutex at a time.
* **Critical Section**: The specific block of code that accesses the shared resource (e.g., `counter++`).
* `std::lock_guard<std::mutex> guard(my_mutex)`: The preferred, safer C++ (RAII) way to lock a mutex. It automatically locks the mutex when created and automatically unlocks it when it goes out of scope, even if an exception is thrown.

---

### Step 6: Using `std::this_thread::yield`

#### Task

Write a program that creates two threads, both running CPU-bound loops (e.g., counting to 10 million). In *one* of the threads, occasionally call `std::this_thread::yield()` inside the loop. In the other, do not. Observe how this changes the behaviour or runtime.

#### Key Concepts

* `std::this_thread::yield()`: A non-blocking function that *hints* to the OS scheduler that the current thread is willing to give up its CPU time slice, allowing another thread to run.
* **Cooperative Scheduling**: `yield()` is a form of cooperative scheduling, where the thread voluntarily pauses itself. This is different from **preemptive scheduling**, where the OS forcibly interrupts a thread.
* **Observation**: The thread that calls `yield()` will likely take *longer* to finish, as it is voluntarily giving up its execution time. This is generally not used for performance, but in complex low-level synchronisation patterns.

---

### Step 7: Using `std::async` and `std::future`

#### Task

Use `std::async` to start a function that calculates the sum of numbers from 1 to 100. This function should return the final sum. Retrieve the result in the main thread using `std::future::get()` and print it.

#### Key Concepts

* `#include <future>`: The header for `std::async` and `std::future`.
* **Task-Based Parallelism**: `std::async` is a higher-level abstraction than `std::thread`. You "launch a task" and get its result back, without manually managing the thread's lifecycle.
* `std::async()`: Launches a function (possibly on a new thread) and immediately returns a `std::future` object.
* `std::future<T>`: An object that *represents the future result* of an operation. `T` is the return type of the function (e.g., `std::future<int>`).
* `my_future.get()`: Blocks the calling thread until the asynchronous task is finished, and then returns the value computed by the task. **You can only call `get()` once.**

---

### Step 8: Measuring Execution Time

#### Task

Use `std::chrono::high_resolution_clock` to measure and compare the time it takes for two worker threads to *each* perform a heavy task (e.g., counting to 100 million). Compare the time taken when the tasks are run **sequentially** (one after the other) vs. **concurrently** (at the same time on different threads).

#### Key Concepts

* `std::chrono::high_resolution_clock`: A clock used for high-precision time measurements.
* `auto start = std::chrono::high_resolution_clock::now();`: Gets a high-precision time point.
* `auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);`: Calculates the difference between two time points and casts it into a specific unit (like milliseconds, microseconds, or seconds).
* `duration.count()`: Returns the numerical value of the duration.
* **Observation**: On a multi-core processor, the concurrent execution time should be significantly faster (ideally, close to half the time) than the sequential execution.

### C++ Threading Quick Reference

| Step | Goal / Concept | Primary C++ Tools | Key Header(s) |
| :--- | :--- | :--- | :--- |
| **1** | **Create & Wait** | `std::thread t(func);`<br>`t.join();` | `<thread>` |
| **2** | **Pass Arguments** | `std::thread(func, arg1, arg2);`<br>`std::ref(var_by_ref);` | `<thread>` |
| **3** | **Pause Thread** | `std::this_thread::sleep_for(ms);` | `<thread>`, `<chrono>` |
| **4** | **Manage Multiple** | `std::vector<std::thread> vec;`<br>`vec.push_back(std::thread(f));` | `<thread>`, `<vector>` |
| **5** | **Protect Shared Data** (Mutex) | `std::mutex mtx;`<br>`std::lock_guard<std::mutex> lock(mtx);` | `<mutex>` |
| **6** | **Cooperative Scheduling** (Hint) | `std::this_thread::yield();` | `<thread>` |
| **7** | **Task-Based Return Value** | `std::future<T> f = std::async(func);`<br>`T result = f.get();` | `<future>` |
| **8** | **Measure Time** | `auto start = high_resolution_clock::now();`<br>`auto ms = duration_cast<ms>(end-start);` | `<chrono>` |