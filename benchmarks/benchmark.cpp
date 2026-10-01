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

template<typename Heap>
double measure(const std::vector<int>& keys,
               const std::vector<int>& expected) {
    Heap heap;
    std::vector<int> output(keys.size());

    const auto start = std::chrono::steady_clock::now();

    for (int key : keys) {
        heap.insert(key);
    }

    for (std::size_t i = 0; i < output.size(); ++i) {
        output[i] = heap.extract_min();
    }

    const auto stop = std::chrono::steady_clock::now();

    // Explicit checks stay active even when assertions are disabled.
    if (output != expected) {
        throw std::runtime_error("Incorrect extraction sequence.");
    }

    if (!heap.empty() || heap.size() != 0 || !heap.validate()) {
        throw std::runtime_error("Invalid heap after draining.");
    }

    return std::chrono::duration<double, std::milli>(
        stop - start).count();
}

std::vector<int> generate_keys(std::size_t n, std::uint32_t seed) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(
        -1'000'000, 1'000'000);

    std::vector<int> keys(n);
    for (int& key : keys) {
        key = distribution(generator);
    }
    return keys;
}

int main() {
    try {
        const std::vector<std::size_t> sizes{
            1'000, 10'000, 100'000
        };

        // Three inputs per size, with five timing repetitions each.
        const std::vector<std::uint32_t> seeds{42, 123, 2026};
        constexpr int repetitions = 5;

        std::cout
            << "workload,heap,n,seed,repetition,elapsed_ms\n"
            << std::fixed << std::setprecision(6);

        for (std::size_t n : sizes) {
            for (std::uint32_t seed : seeds) {
                const auto keys = generate_keys(n, seed);
                auto expected = keys;
                std::sort(expected.begin(), expected.end());

                // Warm-up measurements are discarded.
                measure<FibonacciHeap>(keys, expected);
                measure<IndexedBinaryHeap>(keys, expected);

                for (int repetition = 1;
                     repetition <= repetitions;
                     ++repetition) {

                    auto run_fibonacci = [&]() {
                        const double elapsed =
                            measure<FibonacciHeap>(keys, expected);

                        std::cout
                            << "insert_extract,FibonacciHeap,"
                            << n << ',' << seed << ','
                            << repetition << ',' << elapsed << '\n';
                    };

                    auto run_binary = [&]() {
                        const double elapsed =
                            measure<IndexedBinaryHeap>(keys, expected);

                        std::cout
                            << "insert_extract,IndexedBinaryHeap,"
                            << n << ',' << seed << ','
                            << repetition << ',' << elapsed << '\n';
                    };

                    // Alternate order to reduce consistent order bias.
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

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Benchmark failed: " << error.what() << '\n';
        return 1;
    }
}