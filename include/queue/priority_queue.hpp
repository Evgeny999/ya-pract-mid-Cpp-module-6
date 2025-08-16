#pragma once
#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"
#include "types.hpp"

#include <atomic>
#include <condition_variable>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace dispatcher::queue {

class PriorityQueue {
    std::map<TaskPriority, std::unique_ptr<IQueue>> priorityToQueue;
    std::mutex mtx_;
    bool shutdown_ = true;
    std::condition_variable not_empty_;

public:
    PriorityQueue(std::unordered_map<TaskPriority, QueueOptions>);

    void push(TaskPriority priority, std::function<void()> task);
    // block on pop until shutdown is called
    // after that return std::nullopt on empty queue
    std::optional<std::function<void()>> pop();

    void shutdown();
    // Все очереди пусты (или их вообще нет)
    bool empty();

    ~PriorityQueue();
};

}  // namespace dispatcher::queue