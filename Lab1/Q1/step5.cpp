 #include <iostream>
 #include <thread>
 #include <future>
 #include <chrono>

  /**
  * Step 5 - Thread Synchronisation with std::mutex
  * Write a program where two threads both increment a shared global counter variable 1000 times.
  * First run it without any locking -- what happens?
  * Then add a std::mutex to protect the counter using lock_guard<std::mutex> so the result is correct.
  */

int counter = 0;
std::mutex m;

void add_1000() {
    for (int i = 0; i < 1000; ++i) {
        std::lock_guard <std::mutex> lg(m);
        ++counter;
    }
}

int main() {
    std::thread t1(add_1000), t2(add_1000);
    t1.join(); t2.join();
    std::cout << "counter =" << "(expected 2000)\n";
}