#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include "queue/priority_queue.hpp"
#include "thread_pool/thread_pool.hpp"
#include "types.hpp"

namespace dispatcher {

class TaskDispatcher {
    std::shared_ptr<queue::PriorityQueue> task_queue_;
    thread_pool::ThreadPool thread_poll_;

public:
    TaskDispatcher(size_t thread_count,
                   std::unordered_map<TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions = {
                       {TaskPriority::High, {true, 1000}}, {TaskPriority::Normal, {false, std::nullopt}}});

    void schedule(TaskPriority priority, std::function<void()> task);
    ~TaskDispatcher() = default;
};

}  // namespace dispatcher