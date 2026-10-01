// AI-generated benchmark harness.
// Disclose its generation and your verification in the report.

#include "FibonacciHeap.hpp"
#include "IndexedBinaryHeap.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

struct Update {
    std::size_t item;
    int new_key;
};

struct Workload {
    std::vector<int> initial;
    std::vector<Update> updates;
    std::vector<int> expected;
};

struct Timing {
    double update_ms;
    double total_ms;
};

Workload generate_workload(std::size_t n,
                           std::uint32_t seed,
                           std::size_t updates_per_item) {
    if (n == 0) {
        throw std::invalid_argument("Workload must be nonempty.");
    }

    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> initial_key(
        1'000'000, 2'000'000);
    std::uniform_int_distribution<std::size_t> target(0, n - 1);
    std::uniform_int_distribution<int> decrement(1, 100);

    Workload workload;
    workload.initial.resize(n);

    for (int& key : workload.initial) {
        key = initial_key(generator);
    }

    auto current = workload.initial;
    const std::size_t update_count = n * updates_per_item;
    workload.updates.reserve(update_count);

    for (std::size_t i = 0; i < update_count; ++i) {
        const std::size_t item = target(generator);

        // Use a wider type while calculating to check for overflow.
        const std::int64_t new_key =
            static_cast<std::int64_t>(current[item])
            - decrement(generator);

        if (new_key < -2'000'000'000LL) {
            throw std::runtime_error("Generated key exceeds safe range.");
        }

        current[item] = static_cast<int>(new_key);
        workload.updates.push_back({item, current[item]});
    }

    workload.expected = std::move(current);
    std::sort(workload.expected.begin(), workload.expected.end());
    return workload;
}

template<typename Heap>
Timing measure(const Workload& workload) {
    using Clock = std::chrono::steady_clock;

    Heap heap;
    std::vector<typename Heap::Handle> handles;
    handles.reserve(workload.initial.size());
    std::vector<int> output(workload.initial.size());

    const auto total_start = Clock::now();

    // Every initial item is positive, so 0 is uniquely the minimum.
    heap.insert(0);

    for (int key : workload.initial) {
        handles.push_back(heap.insert(key));
    }

    const int sentinel = heap.extract_min();

    const auto update_start = Clock::now();

    for (const Update& update : workload.updates) {
        heap.decrease_key(handles[update.item], update.new_key);
    }

    const auto update_stop = Clock::now();

    for (std::size_t i = 0; i < output.size(); ++i) {
        output[i] = heap.extract_min();
    }

    const auto total_stop = Clock::now();

    // All correctness checks are outside the measured intervals.
    if (sentinel != 0 || output != workload.expected) {
        throw std::runtime_error("Incorrect extraction sequence.");
    }

    if (!heap.empty() || heap.size() != 0 || !heap.validate()) {
        throw std::runtime_error("Invalid heap after draining.");
    }

    return {
        std::chrono::duration<double, std::milli>(
            update_stop - update_start).count(),
        std::chrono::duration<double, std::milli>(
            total_stop - total_start).count()
    };
}

void print_result(const char* heap_name,
                  std::size_t n,
                  std::uint32_t seed,
                  std::size_t updates_per_item,
                  int repetition,
                  const Timing& timing) {
    std::cout
        << "batch_decrease," << heap_name << ','
        << n << ',' << seed << ','
        << updates_per_item << ',' << repetition << ','
        << timing.update_ms << ',' << timing.total_ms << '\n';
}

int main() {
    try {
        const std::vector<std::size_t> sizes{
            1'000, 10'000, 100'000
        };
        const std::vector<std::uint32_t> seeds{42, 123, 2026};
        const std::vector<std::size_t> update_ratios{1, 10};
        constexpr int repetitions = 5;

        std::cout
            << "workload,heap,n,seed,updates_per_item,"
               "repetition,update_ms,total_ms\n"
            << std::fixed << std::setprecision(6);

        for (std::size_t n : sizes) {
            for (std::uint32_t seed : seeds) {
                for (std::size_t ratio : update_ratios) {
                    const auto workload =
                        generate_workload(n, seed, ratio);

                    measure<FibonacciHeap>(workload);
                    measure<IndexedBinaryHeap>(workload);

                    for (int repetition = 1;
                         repetition <= repetitions;
                         ++repetition) {
                        auto run_fibonacci = [&]() {
                            const auto timing =
                                measure<FibonacciHeap>(workload);
                            print_result("FibonacciHeap", n, seed,
                                         ratio, repetition, timing);
                        };

                        auto run_binary = [&]() {
                            const auto timing =
                                measure<IndexedBinaryHeap>(workload);
                            print_result("IndexedBinaryHeap", n, seed,
                                         ratio, repetition, timing);
                        };

                        if (repetition % 2 == 1) {
                            run_fibonacci();
                            run_binary();
                        } else {
                            run_binary();
                            run_fibonacci();
                        }
                    }
                }
            }
        }

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Benchmark failed: " << error.what() << '\n';
        return 1;
    }
}