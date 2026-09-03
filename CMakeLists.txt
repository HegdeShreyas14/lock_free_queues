cmake_minimum_required(VERSION 3.20)
project(lock_free_queues CXX)

# --- C++ standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # use -std=c++23, not -std=gnu++23

# Default to a Release build if the user didn't specify one.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# std::thread needs the platform threading library.
find_package(Threads REQUIRED)

# Enable `ctest`.
enable_testing()

# --- Normal test target ---
add_executable(test_spsc tests/test_spsc.cpp)
target_include_directories(test_spsc PRIVATE include)
target_link_libraries(test_spsc PRIVATE Threads::Threads)
target_compile_options(test_spsc PRIVATE -Wall -Wextra)
add_test(NAME test_spsc COMMAND test_spsc)

# --- Benchmark target ---
# Optimized build; run manually to get throughput numbers (not a ctest test).
add_executable(benchmark benchmarks/benchmark.cpp)
target_include_directories(benchmark PRIVATE include)
target_link_libraries(benchmark PRIVATE Threads::Threads)
target_compile_options(benchmark PRIVATE -O2)

# --- ThreadSanitizer target ---
# Same test sources, instrumented to detect data races.
# Build + run with:  cmake --build . --target test_spsc_tsan && ./test_spsc_tsan
add_executable(test_spsc_tsan tests/test_spsc.cpp)
target_include_directories(test_spsc_tsan PRIVATE include)
target_link_libraries(test_spsc_tsan PRIVATE Threads::Threads)
target_compile_options(test_spsc_tsan PRIVATE -Wall -Wextra -g -fsanitize=thread)
target_link_options(test_spsc_tsan PRIVATE -fsanitize=thread)
add_test(NAME test_spsc_tsan COMMAND test_spsc_tsan)
