#include "queue/bounded_queue.hpp"

#include <optional>
#include <stdexcept>

namespace dispatcher::queue {

BoundedQueue::BoundedQueue(int capacity) : capacity_(capacity) {}

void BoundedQueue::push(std::function<void()> task) {
    std::lock_guard<std::mutex> lkg(mtx_);
    if (queue_.size() == capacity_) {
        throw std::runtime_error("no empty space in queue left");
    }
    queue_.push(std::move(task));
}

std::optional<std::function<void()>> BoundedQueue::try_pop() {
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