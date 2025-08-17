#include "queue/priority_queue.hpp"
#include <iostream>

namespace dispatcher::queue {
PriorityQueue::PriorityQueue(std::unordered_map<TaskPriority, QueueOptions> priorityToOptions) {
    for (const auto &elem : priorityToOptions) {
        if (elem.second.bounded) {
            if (!elem.second.capacity.has_value()) {
                throw std::runtime_error("Requested bounded queue but capacity is not set");
            }
            priorityToQueue[elem.first] = std::make_unique<BoundedQueue>(elem.second.capacity.value());
        } else {
            priorityToQueue[elem.first] = std::make_unique<UnboundedQueue>();
        }
    }
}

void PriorityQueue::push(TaskPriority priority, std::function<void()> task) {
    std::lock_guard lkg(mtx_);
    auto foundQueue = priorityToQueue.find(priority);
    if (foundQueue == priorityToQueue.end()) {
        throw std::runtime_error("Queue for this priority does not exist");
    }
    // выбросит исключение, если очередь ограничена и полностью заполнена
    foundQueue->second->push(std::move(task));
    not_empty_.notify_one();
}
// block on pop until shutdown is called
// after that return std::nullopt on empty queue
std::optional<std::function<void()>> PriorityQueue::pop() {
    std::unique_lock lock(mtx_);

    //
    // Перед тем как заснуть, метод wait проверит условие, записанное в лямбда-функции
    //     - Если очередь не пустая ИЛИ она уже был shutdown, то выполнение продолжится без ожидания
    //     - Если очередь пустая И она ещё не было shutdown, то поток уснёт, сняв блокировку с мьютекса
    //
    bool statusEmpty = true;
    not_empty_.wait(lock, [this] { return !empty() || !shutdown_; });

    if (empty()) {
        std::cout << "HERE1" << std::endl;
        return std::nullopt;
    }

    //
    // Наша очередь в случае shutdown позволяет обработать находящиеся внутри неё данные,
    // поэтому после wait мы не проверяем флаг shutdown_
    //

    // Извлекаем элемент из очереди с наибольшим приоритетом
    std::cout << "HERE2" << std::endl;

    std::optional<std::function<void()>> result;

    for (auto it = priorityToQueue.begin(); it != priorityToQueue.end(); ++it) {
        // Если очередь для элементов с определённым приоритетов не пуста, то пытаемся сделать pop для неё
        if (!it->second->empty()) {
            result = std::move(it->second->try_pop());
            break;
        }
    }
    /*auto &q = *priorityToQueue.rbegin();
    auto result = std::move(q.second->try_pop());*/

    return result;
}

void PriorityQueue::shutdown() {
    std::lock_guard lkg(mtx_);
    shutdown_ = false;
    not_empty_.notify_all();
}

bool PriorityQueue::empty() {
    bool statusEmpty = true;
    for (const auto &q : priorityToQueue) {
        statusEmpty = q.second->empty();
        if (!statusEmpty) {
            break;
        }
    }
    return statusEmpty;
}
}  // namespace dispatcher::queue
