#include "thread_pool/thread_pool.hpp"

#include <algorithm>
#include <ranges>

namespace dispatcher::thread_pool {

ThreadPool::ThreadPool(std::shared_ptr<dispatcher::queue::PriorityQueue> taskQueue, size_t num_threads)
    : taskQueue_(taskQueue) {
    workers_.reserve(num_threads);
    for (size_t i = 0; i < num_threads; ++i) {
        // специальный синтаксис для запуска потоков с помощью метода класса
        workers_.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    taskQueue_->shutdown();
    /*jthread должны сами присоединиться, делать тут вроде ничего не нужно*/
}

void ThreadPool::worker() {
    while (1) {
        // блокируется, если задач нет и не было shutdown
        std::optional<std::function<void()>> task = taskQueue_->pop();
        // если получили реальную задачу, то выполняем ее
        if (task.has_value()) {
            task.value()();
        } else {
            // pop вернул nullopt, значит был shutdown
            break;
        }
    }
}

}  // namespace dispatcher::thread_pool
