#pragma once
#include "queue/queue.hpp"
#include <mutex>
#include <queue>

namespace dispatcher::queue {

class BoundedQueue : public IQueue {
    std::queue<std::function<void()>> queue_;
    std::mutex mtx_;
    int capacity_;

public:
    explicit BoundedQueue(int capacity);

    void push(std::function<void()> task) override;

    std::optional<std::function<void()>> try_pop() override;

    bool empty() override { return queue_.empty(); }

    ~BoundedQueue() override = default;
};

}  // namespace dispatcher::queue