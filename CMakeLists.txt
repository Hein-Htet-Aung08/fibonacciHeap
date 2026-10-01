cmake_minimum_required(VERSION 3.16)
project(FibonacciHeapStudy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(heaps STATIC
    src/FibonacciHeap.cpp
    src/IndexedBinaryHeap.cpp
)

target_include_directories(heaps PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(heap_tests tests/heap_tests.cpp)
target_link_libraries(heap_tests PRIVATE heaps)

# Tests use assert(), so keep assertions enabled in Release builds too.
if(MSVC)
    target_compile_options(heap_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(heap_tests PRIVATE -UNDEBUG)
endif()

enable_testing()
add_test(NAME heap_correctness COMMAND heap_tests)

add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark PRIVATE heaps)

add_executable(decrease_benchmark benchmarks/decrease_benchmark.cpp)
target_link_libraries(decrease_benchmark PRIVATE heaps)

add_executable(large_decrease_benchmark
    benchmarks/large_decrease_benchmark.cpp
)
target_link_libraries(large_decrease_benchmark PRIVATE heaps)