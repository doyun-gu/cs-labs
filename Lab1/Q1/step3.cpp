 #include <iostream>
 #include <thread>
 #include <future>
 #include <chrono>

 /**
 * Step 3 - Use std::this_thread::sleep_for
 * Modify your Lab 2 program so that each number if printed 200m s apart using std::this_thread::sleep_for (std::chrono::miliseconds(200));
 * Oberserve how this affects the timing of the output.
 */

void print_numbers_slow (int n) {
    using namespace std::chrono_literals;

    for (int i = 1; i <= n; ++i) {
        std::cout << i << "" << std::flush;
        std::this_thread::sleep_for(200ms);
    }

    std::cout<<"\n";
}

int main() {
    std::thread t(print_numbers_slow, 10);
    t.join();
}