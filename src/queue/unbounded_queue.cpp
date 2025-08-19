#include "queue/unbounded_queue.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

void UnboundedQueue::push(std::function<void()> task) {
    std::lock_guard<std::mutex> lkg(mtx_);
    queue_.push(std::move(task));
}

std::optional<std::function<void()>> UnboundedQueue::try_pop() {
    std::lock_guard<std::mutex> lkg(mtx_);
    if (queue_.size() > 0) {
        auto frontElement = queue_.front();
        queue_.pop();
        return frontElement;
    } else {
        return std::nullopt;
    }
}

}  // namespace dispatcher::queue