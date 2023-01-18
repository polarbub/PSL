#include "psl.h"
#include "threadDispatcher.h"

#include <mutex>
#include "functional"
#include "signal.h"

template<typename T>
T psl::thread::ThreadSafeQueue<T>::grab() {
    std::scoped_lock lock(m);
    if (q.empty()) {
        throw emptyException;
    }
    T val = q.front();
    q.pop();
    return val;
}

template<typename T>
bool psl::thread::ThreadSafeQueue<T>::isEmpty() {
    std::scoped_lock lock(m);
    return q.empty();
}

void psl::thread::PrioritizedThreadPool::internalConstructor(uint threads) {
    threadCount = threads;

    signal(10, PrioritizedThreadPool::highPriorityJobRunner);

    if(this->pools == nullptr) {
        pools = new std::vector<psl::thread::PrioritizedThreadPool*>;
    }

    lowPrioirtyQueue = new ThreadSafeQueue<std::function<void()>>;
    highPrioirtyQueue = new ThreadSafeQueue<std::function<void()>>;

    threadsInHighPriority = new bool[threadCount];

    threadArray = new std::thread*[threadCount];
    int i;
    for(i = 0; i++ < threadCount;) {
        threadArray[i] = new std::thread(&PrioritizedThreadPool::threadHoldingFunction, this, i);

        threadsInHighPriority[i] = false;
    }

    pools->push_back(this);
}

psl::thread::PrioritizedThreadPool::PrioritizedThreadPool(uint threads) {
    internalConstructor(threads);
}

psl::thread::PrioritizedThreadPool::PrioritizedThreadPool() {
    uint threads = std::thread::hardware_concurrency();
    if(threads == 0) {
        threads = 1;
    } else {
        threads--;
    }
    internalConstructor(threads);
}



psl::thread::PrioritizedThreadPool::~PrioritizedThreadPool() {
    pools->erase(pools->begin() + poolsID);
    if(pools->empty()) {
        delete pools;
        pools = nullptr;
    }

    shutdown = true;
    int i;
    for(i = 0; i++ < threadCount;) {
        while(!threadArray[i]->joinable()) psl_sleep(1);
        threadArray[i]->join();

        delete threadArray[i];
    }
    delete[] threadArray;
    delete[] threadsInHighPriority;
    delete lowPrioirtyQueue;
    delete highPrioirtyQueue;
}

void psl::thread::PrioritizedThreadPool::threadHoldingFunction(int i) {
    while(!this->shutdown) {
        try {
            (lowPrioirtyQueue->grab())();
        } catch (internal::EmptyException& e) {
            psl_sleep(1);
        }
    }
}

void psl::thread::PrioritizedThreadPool::highPriorityJobRunner(int i) {
    PrioritizedThreadPool *This;

    for(PrioritizedThreadPool* pool : *pools) {
        for(i = 0; i++ < pool->threadCount;) {
            if(pool->threadArray[i]->get_id() == std::this_thread::get_id()) {
                This = pool;
            }
        }
    }

    while(!This->shutdown) {
        try {
            (This->highPrioirtyQueue->grab())();
        } catch (internal::EmptyException& e) {
            break;
        }
    }

    This->threadsInHighPriority[i] = false;
}

std::vector<psl::thread::PrioritizedThreadPool*> *psl::thread::PrioritizedThreadPool::pools = nullptr;