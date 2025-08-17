#include "task_dispatcher.hpp"

namespace dispatcher {

TaskDispatcher::TaskDispatcher(size_t thread_count,
                               std::unordered_map<TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions)
    : task_queue_(std::make_shared<queue::PriorityQueue>(priorityToOptions)),
      thread_poll_(thread_pool::ThreadPool{task_queue_, thread_count}) {}

void TaskDispatcher::schedule(TaskPriority priority, std::function<void()> task) { task_queue_->push(priority, task); }

}  // namespace dispatcher