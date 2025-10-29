/**
 * Non-assessed code!
 * Cyclic Executive Example
 *
 * This example of a flexible cyclic execution that has a 10 ms minor cycle
 * and a 100 ms major cycle, including jitter and overrun detection.
 * It has some nice features:
 * 1. Time-triggered minor/major cycles: tasks run only at their scheduled release times.
 * 2. Strict periodic release & jitter measurement: logs worst release jitter vs schedule.
 * 3. WCET (Worst-Case Execution Time) budget & overrun count: flags when simulated work
 * exceeds a task’s budget. Knowing each task’s WCET is essential so you
 * can guarantee that all tasks finish within their assigned time slots.
 * 4. Sleep-until frame boundary: Avoids busy waiting; shows frame-overrun
 * warnings if you overload the frame.
 */

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include <cmath> // For std::llabs

// Type aliases for clarity
using Clock = std::chrono::steady_clock;
using ms = std::chrono::milliseconds;
using us = std::chrono::microseconds;

// --- Utilities to simulate work and measure timing ---

/**
 * @brief Simulate work by spinning for a target number of microseconds.
 * @param target_us The approximate number of microseconds to spin.
 */
void busy_work_us(int target_us) {
    auto start = Clock::now();
    while (std::chrono::duration_cast<us>(Clock::now() - start).count() < target_us) {
        // This loop intentionally consumes CPU to simulate execution time.
        // In a real x86 system, you might use _mm_pause() here
        // to be friendlier to hyper-threading.
    }
}

/**
 * @brief Holds execution statistics for a single task.
 */
struct RunStats {
    int runs = 0;
    long long worst_jitter_us = 0;
    long long worst_exec_us = 0;
    int overruns = 0;
};

/**
 * @brief Defines a single periodic task for the executive.
 */
struct Task {
    std::string name;
    ms period;              // How often to execute
    ms phase{0};            // Initial delay before first release
    us wcet_budget;         // Execution budget (microseconds)
    std::function<void()> work; // The task body
    Clock::time_point next_release; // When the task is next due
    RunStats stats{};
};

// ------------------------------------------------------------------
// Schedule table for a 10 ms minor cycle (time-triggered)
// ------------------------------------------------------------------
int main() {
    // Enable C++ chrono literals (e.g., 10ms, 2000us)
    using namespace std::chrono_literals;

    const ms minor_cycle{10};     // 10 ms frame
    const ms major_cycle{100};    // 100 ms (10 frames)
    const int major_cycles_to_run = 5; // demo runtime ≈ 0.5 s
    const ms slip_tolerance{1}; // Allowable slip before warning

    // Define tasks directly in the vector using C++20 aggregate initialization
    std::vector<Task> tasks{
        {
            .name = "SensorRead",
            .period = 10ms,
            .phase = 0ms,
            .wcet_budget = 2000us, // 2 ms
            .work = [] { busy_work_us(1500); }
        },
        {
            .name = "Control",
            .period = 20ms,
            .phase = 0ms,
            .wcet_budget = 3000us, // 3 ms
            .work = [] { busy_work_us(2200); }
        },
        {
            .name = "CommTx",
            .period = 50ms,
            .phase = 0ms,
            .wcet_budget = 5000us, // 5 ms
            .work = [] { busy_work_us(3500); }
        }
    };

    // Initialise next releases relative to executive start
    const auto t0 = Clock::now();
    for (auto& t : tasks) {
        t.next_release = t0 + t.phase;
    }

    std::cout << "Cyclic Executive: minor=" << minor_cycle.count()
              << " ms, major=" << major_cycle.count() << " ms\n";

    auto frame_start = t0;
    const auto end_time = t0 + major_cycle * major_cycles_to_run;
    int frame_idx = 0;

    // --- Main Executive Loop ---
    while (Clock::now() < end_time) {
        // 1) Release and execute due tasks within this frame
        auto now = Clock::now();
        for (auto& t : tasks) {
            if (now >= t.next_release) {
                // Measure release jitter vs exact schedule point
                const auto jitter_us =
                    std::chrono::duration_cast<us>(now - t.next_release).count();
                if (std::llabs(jitter_us) > t.stats.worst_jitter_us) {
                    t.stats.worst_jitter_us = std::llabs(jitter_us);
                }

                // Execute task and measure execution time
                const auto exec_start = Clock::now();
                t.work();
                const auto exec_us =
                    std::chrono::duration_cast<us>(Clock::now() - exec_start).count();

                // Update execution stats
                if (exec_us > t.stats.worst_exec_us) {
                    t.stats.worst_exec_us = exec_us;
                }
                if (exec_us > t.wcet_budget.count()) {
                    t.stats.overruns++;
                }
                t.stats.runs++;

                // Schedule next release (strict periodic)
                t.next_release += t.period;

                // Catch up if we slipped multiple periods
                while (t.next_release < now) {
                    t.next_release += t.period;
                }
            }
        }

        // 2) Sleep until the next frame boundary (non-busy)
        frame_idx = (frame_idx + 1) % (major_cycle / minor_cycle);
        frame_start += minor_cycle;
        std::this_thread::sleep_until(frame_start);

        // Optional: detect frame overrun (if tasks exceeded frame budget)
        auto after_sleep = Clock::now();
        if (after_sleep > frame_start + slip_tolerance) {
            auto slip = std::chrono::duration_cast<us>(after_sleep - frame_start).count();
            // Use std::cerr for warnings
            std::cerr << "[WARN] Frame overrun: slipped by " << slip << " us\n";
        }
    }

    // ------------------------------------------------------------------
    // Report
    // ------------------------------------------------------------------
    std::cout << "\n=== Report ("
              << major_cycles_to_run << " majors of "
              << major_cycle.count() << " ms) ===\n";

    for (const auto& t : tasks) { // Use const& for read-only access
        std::cout << "Task " << std::left << std::setw(10) << t.name
                  << " runs=" << std::setw(4) << t.stats.runs
                  << " worst_jitter=" << std::setw(6) << t.stats.worst_jitter_us << " us"
                  << " worst_exec=" << std::setw(6) << t.stats.worst_exec_us << " us"
                  << " overruns=" << t.stats.overruns
                  << "\n";
    }
    std::cout << "Done.\n";
}