# Lock-Free-Queues

Lock-free queues in C++23, built to understand the memory-ordering
and cache-behaviour details that make single-writer concurrent data structures
correct and fast.

This project implements a lock-free single producer single consumer (SPSC) queue and benchmarks it against the conventional queue which uses mutexes

## Design

- **Bounded ring buffer** backed by a fixed-size array.
- **Two atomic indices** — the producer owns `tail`, the consumer owns `head`;
  neither writes the other's index, which is what removes the need for any
  compare-and-swap or locking.
- **Acquire/release ordering** so the consumer never observes an advanced
  `tail` before the slot write that produced it (and symmetrically for the
  producer).
- **Cache-line-aligned indices** (`alignas`) so the producer's writes to `tail`
  don't invalidate the cache line holding the consumer's `head` — avoiding
  false sharing.
- **One reserved slot** so `head == tail` means empty and
  `next(tail) == head` means full, with no separate size counter.

## Correctness

The queue is tested under single-threaded (fill/empty behaviour, FIFO ordering,
index wraparound) and a two-thread stress test that moves 1,000,000
items and verifies none are lost, duplicated, or reordered. The stress test
also runs clean under **ThreadSanitizer** (`-fsanitize=thread`), which checks
the memory ordering is correct and not just passing by luck.

## Performance

On an AMD Ryzen 9 8945HX (g++ 13, `-O2`), moving 10M items through the queue:

| Queue | Throughput | Per item |
|---|---|---|
| `std::mutex` + `std::queue` | ~16 M items/sec | ~62 ns |
| lock-free SPSC | ~161 M items/sec | ~6 ns |

**~10x faster** at the median, and with far lower run-to-run variance. Full
methodology and caveats in [`results/benchmark.md`](results/benchmark.md).

## Build & run

```
cmake -S . -B build
cmake --build build

cd build
ctest --output-on-failure   # run the test suite
./benchmark                 # run the throughput benchmark
```

Requires a C++23 compiler and CMake ≥ 3.20.

To run the ThreadSanitizer build:

```
cmake --build build --target test_spsc_tsan
./build/test_spsc_tsan
```

## Layout

```
include/      spsc_queue.hpp      queue header file
tests/        test_spsc.cpp       tests
benchmarks/   benchmark.cpp       throughput vs mutex baseline
results/      benchmark.md        benchmark
```
