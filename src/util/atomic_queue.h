#pragma once

#include <cstddef>
#include <array>
#include <atomic>
#include <semaphore>


template<typename T, std::size_t bufferSize>
class AtomicQueue
{
    private:
        std::array<T, bufferSize> buffer{};
        std::atomic_size_t writeIndex{0};
        std::atomic_size_t readIndex{0};


    public:
        explicit AtomicQueue() {}
        AtomicQueue(const AtomicQueue&) = delete;
        AtomicQueue& operator=(const AtomicQueue&) = delete;
        AtomicQueue(AtomicQueue &&arr) = delete;
        AtomicQueue& operator=(AtomicQueue &&arr) = delete;

        std::counting_semaphore<1> signal = std::counting_semaphore<1>{0};

        void push(T&& value)
        {
            const size_t prevIndex = writeIndex.fetch_add(1, std::memory_order_relaxed);
            buffer[prevIndex % bufferSize] = value;
            signal.release();
        }

        T& pop()
        {
            const size_t prevIndex = readIndex.fetch_add(1, std::memory_order_relaxed);
            return buffer[prevIndex % bufferSize];
        }

        void wait(){ signal.acquire(); }


}; // class AtomicQueue