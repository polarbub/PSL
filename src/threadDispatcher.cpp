#include "psl.h"
#include "threadDispatcher.h"

template<typename T>
void psl::Thread::ThreadSafeQueue<T>::queue(T t) {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
}

template<typename T>
T psl::Thread::ThreadSafeQueue<T>::grab() {
    std::unique_lock<std::mutex> lock(m);
    if (q.empty()) {
        return nullptr;
    }
    T val = q.front();
    q.pop();
    return val;
}