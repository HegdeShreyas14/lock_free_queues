#include "spsc_queue.hpp"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <queue>
#include<algorithm>
#include <thread>
constexpr int kCount = 10'000'000;
constexpr std::size_t kCapacity = 1024;
template <typename F>
double time_seconds(F&& fn) {
    const auto start = std::chrono::steady_clock::now();
    fn();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

//helper, turns elapsed time into a report line
void report(const char* name, double seconds) {
    const double ops_per_sec = kCount / seconds;
    const double ns_per_op   = (seconds * 1e9) / kCount;
    std::cout<<name<<":\n"<<" time:     "<<seconds<<" s\n"<<" throughput: "<<(ops_per_sec/1e6)<<" M items/sec\n"<<" per item: "<<ns_per_op<<" ns\n\n";
}

double bench_mutex_queue() {
    std::queue<int> q;
    std::mutex m;
    std::atomic<bool> done{false};

    return time_seconds([&] {
        std::thread producer([&] {
            for (int i = 0; i < kCount; ++i) {
                std::lock_guard<std::mutex> lock(m);
                q.push(i);
            }
        });

        std::thread consumer([&] {
            int received = 0;
            while (received < kCount) {
                std::lock_guard<std::mutex> lock(m);
                while (!q.empty()) {
                    q.pop();
                    ++received;
                }
            }
        });

        producer.join();
        consumer.join();
        (void)done;
    });
}

double bench_spsc_queue() {
    SPSCQueue<int, kCapacity> q;

    return time_seconds([&] {
        std::thread producer([&]{
            for (int i = 0; i < kCount; ++i) {
                            while (!q.push(i)) {
                                // Spin until there is space.
                            }
                        }
        });
        std::thread consumer([&] {
                    for (int i = 0; i < kCount; ++i) {
                        while (!q.pop().has_value()) {
                            // Spin until an item is available.
                        }
                    }
                });

        producer.join();
        consumer.join();
    });
}

int main() {
    std::cout << "Pushing " << kCount << " items through each queue"<<"(SPSC capacity "<< kCapacity << ").\n\n";
    bench_mutex_queue();
    bench_spsc_queue();
    double mutex_best = 1e9, spsc_best = 1e9;
    for (int run = 0; run < 5; ++run) {
        mutex_best = std::min(mutex_best, bench_mutex_queue());
        spsc_best  = std::min(spsc_best,  bench_spsc_queue());
    }
    report("mutex + std::queue", mutex_best);
    report("lock-free SPSCQueue", spsc_best);

    std::cout << "SPSC is " << (mutex_best / spsc_best)
              << "x faster than the mutex baseline.\n";
    return 0;
}
