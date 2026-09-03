# SPSC Queue — Throughput Benchmark

Single-producer / single-consumer hand-off throughput: the lock-free
`SPSCQueue` versus a `std::queue` guarded by a `std::mutex`.

## Setup

| | |
|---|---|
| CPU | AMD Ryzen 9 8945HX (16 cores / 32 threads) |
| Compiler | g++ 13.3.0 (Ubuntu 24.04) |
| Flags | `-O2 -std=c++23` |
| Workload | 10,000,000 `int` items pushed through the queue, one producer thread → one consumer thread |
| SPSC capacity | 1024 (the queue is meant to stay near-empty in steady state, not buffer all 10M items) |
| Runs | 6 timed runs after a warm-up run; figures below are the **median** |

## Results (median of 6 runs)

| Queue | Throughput | Per item | Spread (per item) |
|---|---|---|---|
| `std::mutex` + `std::queue` | ~16 M items/sec | ~62.6 ns | 55 – 76 ns |
| lock-free `SPSCQueue` | ~161 M items/sec | ~6.2 ns | 5.7 – 7.0 ns |

**The lock-free queue is ~10x faster** than the mutex baseline at the median
(observed range across runs: 8x – 12.8x).

## What the numbers mean

The mutex baseline pays a lock/unlock on every operation, and a 
mutex puts the waiting thread to sleep and later wakes it — both involve the
kernel. The lock-free queue coordinates entirely through two atomic indices
with acquire/release ordering and never enters the kernel on the hot path,
which is where the ~10x comes from.

Just as notable as the median is the **variance**: the lock-free queue is far
more consistent (5.7–7.0 ns, a ~1.3 ns spread) than the mutex (55–76 ns, a
~20 ns spread). The mutex's variability comes from the OS scheduler deciding
when sleeping threads wake; the lock-free path avoids the scheduler, so its
latency is both lower and more predictable. For latency-sensitive systems,
predictable tail latency often matters more than average throughput.



## Reproduce

```
cmake -S . -B build
cmake --build build
./build/benchmark
```
