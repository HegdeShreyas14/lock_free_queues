#pragma once
#include<array>
#include<atomic>
#include<cstddef>
#include<new>
#include<optional>

template <typename T, std::size_t Capacity>
class SPSCQueue{
    static_assert( Capacity >= 1 , "Capacity must be 1 at minimum");

private:
    static constexpr std::size_t kCacheLine = 64;
    std::array<T,Capacity + 1> buffer_{}; //varName_ = member variable (per-instance), not local/global
    alignas(kCacheLine) std::atomic<std::size_t> tail_{0};
    alignas(kCacheLine) std::atomic<std::size_t> head_{0};

    static constexpr std::size_t next(std::size_t idx) noexcept{
        return (idx + 1)%(Capacity + 1);
    }

public:
    SPSCQueue() = default;
    SPSCQueue(const SPSCQueue&) = delete;
    SPSCQueue& operator = (const SPSCQueue&) = delete;


    bool push(const T& val) noexcept{ // pushing elements only for the producer
        const std::size_t tail = tail_.load(std::memory_order_relaxed);
        const std::size_t next_tail = next(tail);
        if(next_tail == head_.load(std::memory_order_acquire))
            return false; // if queue full then returns false

        buffer_[tail] = val;

        tail_.store(next_tail , std::memory_order_release);
        return true;
    }

    std::optional<T> pop() noexcept { // popping only for the consumer , if empty then returns std::nullopt
        const std::size_t head = head_.load(std::memory_order_relaxed);
        if(head == tail_.load(std::memory_order_acquire)){
            return std::nullopt;
        }
        T result = buffer_[head];             // read the value OUT (this was missing)
        head_.store(next(head), std::memory_order_release);
        return result;
    }
};
