#pragma once
#include "queue/queue.hpp"

#include <mutex>
#include <queue>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue {
    std::queue<std::function<void()>> queue_;
    // Хотя с bounded и unbounded очередями разодает потокобезопасная PriorityQueue, защитил их тоже мьютексом
    std::mutex mtx_;

public:
    // Зачем capacity в неограниченной очереди?
    // explicit UnboundedQueue(int capacity);

    void push(std::function<void()> task) override;

    std::optional<std::function<void()>> try_pop() override;

    bool empty() override { return queue_.empty(); }

    ~UnboundedQueue() override = default;
};

}  // namespace dispatcher::queue