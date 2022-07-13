#pragma once

#include <csignal>
#include "queue"
#include "thread"
#include "future"
#include "iostream"

#include "functional"

namespace psl::thread {
    class threadSafePrint {
    public:
        explicit threadSafePrint() {};

        template <typename... T>
        void print(const T&... items) {
            const std::scoped_lock lock(stream_mutex);
            (std::cout << ... << items);
        }

        template <typename... T>
        void println(const T&... items) {
            print(items..., '\n');
        }

    private:
        //Keep it thread safe
        mutable std::mutex stream_mutex = {};
    };

    namespace internal {
        class EmptyException: public std::exception
        {
            virtual const char* what() const throw()
            {
                return "The queue was empty";
            }
        };
    }

    template <typename T>
    class ThreadSafeQueue {
    public:
        internal::EmptyException emptyException;

        ThreadSafeQueue(void) : q(), m() {}
        ~ThreadSafeQueue(void) {}

        // Add an element to the queue.
        void queue(T t) {
            std::scoped_lock lock(m);
            q.push(t);
        };

        // Get the "front"-element.
        // If the queue is empty return nullptr
        T grab();

        bool isEmpty();

    private:
        std::queue<T> q;
        mutable std::mutex m;
    };

    class PrioritizedThreadPool {
    public:
        PrioritizedThreadPool(uint threads);
        ~PrioritizedThreadPool();

        template <typename FunctionType, typename... ArgTypes, typename ReturnType = std::invoke_result_t<std::decay_t<FunctionType>, std::decay_t<ArgTypes>...>>
        std::future<ReturnType> submit(const FunctionType& function, const ArgTypes &... args) {
            std::future<ReturnType> returnHandle;
            this->lowPrioirtyQueue->queue(createJob(function, args..., returnHandle));
            return returnHandle;
        };

        template <typename FunctionType, typename... ArgTypes, typename ReturnType = std::invoke_result_t<std::decay_t<FunctionType>, std::decay_t<ArgTypes>...>>
        std::future<ReturnType> submitHighPriority(const FunctionType& function, const ArgTypes &... args) {
            std::future<ReturnType> returnHandle;
            this->highPrioirtyQueue->queue(createJob(function, args..., returnHandle));

            int i;
            for(i = 0; i++ < this->threadCount;) {
                if(!threadsInHighPriority[i]) {
                    //This sends an interrupt to a thread, it doesn't kill it.
                    pthread_kill(threadArray[i]->native_handle(), 10);
                    threadsInHighPriority[i] = true;
                }
            }

            return returnHandle;
        };

    private:
        template <typename FunctionType, typename... ArgTypes, typename ReturnType = std::invoke_result_t<std::decay_t<FunctionType>, std::decay_t<ArgTypes>...>>
        std::function<void()> createJob(const FunctionType& function, const ArgTypes &... args,  std::future<ReturnType>& returnHandle) {
            std::function<ReturnType(ArgTypes...)> realFunction(function);
            std::shared_ptr<std::promise<ReturnType>> promise = std::make_shared<std::promise<ReturnType>>();

            std::function<void()> toCallInternal([args..., realFunction, promise, function] {
                try {
                    if constexpr (std::is_void_v<ReturnType>) {
                        realFunction(args...);
                        promise->set_value();
                    } else {
                        promise->set_value(realFunction(args...));
                    }
                } catch (...) {
                    try {
                        promise->set_exception(std::current_exception());
                    } catch (...) {}
                }
            });
            returnHandle = promise->get_future();
            return toCallInternal;
        };

        bool shutdown = false;

        std::thread** threadArray;
        bool* threadsInHighPriority;
        uint threadCount;

        ThreadSafeQueue<std::function<void()>>* lowPrioirtyQueue;
        ThreadSafeQueue<std::function<void()>>* highPrioirtyQueue;

        int poolsID = 0;
        static std::vector<PrioritizedThreadPool*> *pools;

        void threadHoldingFunction(int i);
        static void highPriorityJobRunner(int i);
    };
}