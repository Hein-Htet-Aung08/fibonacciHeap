# Fibonacci Heap Implementation and Empirical Study

This C++17 project implements a Fibonacci min-heap and compares it with an indexed binary min-heap for Advanced Algorithms Programming Assignment 1, Track A.

Both heaps support insertion, minimum lookup, minimum extraction, decrease-key through handles, size, and emptiness checks. Meld and arbitrary deletion are outside the scope of this study.

## Requirements

- A C++17 compiler, such as Apple Clang or GCC
- CMake 3.16 or newer
- A build tool supported by CMake, such as Make

The reported measurements were collected on an Apple M3 Pro with 18 GB unified memory, macOS 26.5.1, Apple Clang 21.0.0, and CMake 4.4 using the Unix Makefiles generator. The Release compilation used `-O3 -DNDEBUG -std=c++17 -arch arm64 -Wall -Wextra -Wpedantic`. Release flags may differ on another compiler.

On macOS with Homebrew, install CMake if necessary:

```sh
brew install cmake
```

## Build, run benchmarks, and test

Run these commands from the repository root, proceeding only if each command succeeds:

```sh
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The first command configures the project. The second compiles the library and executables and automatically runs all three benchmarks through the `run_benchmarks` target. The third runs the registered correctness tests.

**Every successful default build regenerates the benchmark CSVs in `results/`, even when the source files have not changed.** The directory is created if missing, and existing CSVs are overwritten. Configuration alone does not run benchmarks.

The assertion-based test executable retains its assertions in Release builds. Benchmark correctness checks use explicit exceptions and also remain active.

If an existing build directory uses a different generator, choose a fresh build-directory name in all three commands instead of reusing it.

### Generated results

| File | Experiment | Data rows |
|---|---|---:|
| `results/insert_extract.csv` | Insert N keys and extract all N | 90 |
| `results/batch_decrease.csv` | Small random decreases, followed by extraction | 180 |
| `results/large_decrease.csv` | Decreases that make each target the new minimum, followed by extraction | 180 |

On macOS/Linux, check the line counts with:

```sh
wc -l results/*.csv
```

Expected counts including headers are 91, 181, and 181, totalling 453 lines. A failed run can leave a partial CSV; line counts alone do not replace checking that the build and benchmarks completed successfully.

To rerun only the benchmark target:

```sh
cmake --build build --target run_benchmarks
```

To build and run only the correctness tests without refreshing CSVs:

```sh
cmake --build build --target heap_tests
ctest --test-dir build --output-on-failure
```

For this Unix Makefiles build, individual benchmark executables can also be run directly:

```sh
./build/benchmark
./build/decrease_benchmark
./build/large_decrease_benchmark
```

Direct execution prints CSV text to the terminal. The automatic CMake target redirects that output into the files listed above.

## Report measurements versus fresh measurements

- `resultsUsedInReport/` contains the fixed CSV snapshots used for the report's numerical results and figures. These files are included in the submission.
- `results/` contains newly generated measurements and can be regenerated from the source code.

Building the project does not modify `resultsUsedInReport/`. The marker can inspect those snapshots immediately or build the project to obtain fresh results. New timings will vary with hardware, compiler settings, and machine conditions.

## Repository layout

| Directory or file | Purpose |
|---|---|
| `include/` | Public heap interfaces |
| `src/` | Fibonacci and indexed binary heap implementations |
| `tests/heap_tests.cpp` | Correctness tests for both heaps |
| `benchmarks/benchmark.cpp` | Insert–extract harness |
| `benchmarks/decrease_benchmark.cpp` | Small-decrease harness |
| `benchmarks/large_decrease_benchmark.cpp` | Large-decrease harness |
| `resultsUsedInReport/` | Fixed report measurements |
| `results/` | Generated CSVs from fresh runs |
| `report/` | Written report materials |
| `CMakeLists.txt` | Library, executable, test, and benchmark-run targets |

## Experimental setup

All experiments use N = 1,000, 10,000, and 100,000, with seeds 42, 123, and 2026. Each configuration has one discarded warm-up per heap and five measured repetitions using fresh heaps. Execution order alternates between heaps; with five repetitions, Fibonacci runs first three times and binary twice.

Both heaps receive identical input within each experiment. Input generation and reference sorting occur outside timing. Timing uses `std::chrono::steady_clock`, with results recorded in milliseconds.

- **Insert–extract:** generate keys from −1,000,000 to 1,000,000, insert them, then extract all items.
- **Small decreases:** insert N positive keys and sentinel 0, extract the sentinel, then apply N or 10N randomly targeted decreases of 1–100 from each target's current key before draining the heap.
- **Large decreases:** use the same setup but assign keys −1, −2, −3, and so on. Every updated target becomes the new minimum.

The sentinel extraction consolidates the Fibonacci heap before updates. Updates are applied in a batch without intervening extractions. N and 10N specify total update counts, not exact counts per item.

Decrease workloads record update-phase and total time separately. Total time includes insertion, sentinel extraction, updates, and draining. Internal heap allocations and handle recording are included in the corresponding measured operations. Output allocation, correctness checks, and heap destruction are excluded.

Each harness compares the extracted sequence against sorted expected keys. Decrease workloads compute those expected keys after applying the recorded updates to a reference vector. The harnesses also check the drained heap's state and the sentinel result where applicable.

Report summaries use the median of five repetitions for each seed, then the median of the three seed medians. Plot markers represent seed medians; range bars show their minimum and maximum, not confidence intervals.

## Findings and scope

In the report measurements, the indexed binary heap is faster for insert–extract and for the complete workloads tested. Fibonacci is faster during the large-decrease update phase, but its remaining costs outweigh that saving overall. See the report for figures, analysis, and limitations.

Handles must only be used with the heap that created them. Extraction invalidates the removed item's handle. IDs are not reused, and bookkeeping retains slots for earlier insertions. Both heaps disable copying. Fibonacci consolidation does not fully handle allocation failure.

## AI assistance

ChatGPT assisted with Fibonacci implementation guidance and explanations, generated the indexed binary baseline and benchmark harnesses, and assisted with CMake, documentation, and plotting code. The report details how the generated material was checked, specific AI issues encountered, and what was understood versus taken on trust.
