#pragma once

#include "queue/priority_queue.hpp"

#include <thread>

namespace dispatcher::thread_pool {

class ThreadPool {
    void worker();

    std::shared_ptr<dispatcher::queue::PriorityQueue> taskQueue_;
    std::vector<std::jthread> workers_;

public:
    explicit ThreadPool(std::shared_ptr<dispatcher::queue::PriorityQueue> taskQueue,
                        size_t num_threads = std::thread::hardware_concurrency());

    ~ThreadPool();
};

}  // namespace dispatcher::thread_pool