#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <thread>
#include <any>

namespace psl::Thread {
    template <typename T>
    class ThreadSafeQueue {
    public:
        ThreadSafeQueue(void) : q(), m(), c() {}
        ~ThreadSafeQueue(void) {}

        // Add an element to the queue.
        void queue(T t);

        // Get the "front"-element.
        // If the queue is empty return nullptr
        T grab();

    private:
        std::queue<T> q;
        mutable std::mutex m;
        std::condition_variable c;
    };

    class ThreadDispatcher {
    public:
        ThreadDispatcher (uint threads) {

        }
    private:
        std::thread* threadArray;
        uint threadCount;
        ThreadSafeQueue<std::future<std::any>>* lowPrioirtyQueue;
        ThreadSafeQueue<std::future<std::any>>* highPrioirtyQueue;

        void threadHoldingFunction() {

        }
    };
}