#include <gtest/gtest.h>

#include "task_dispatcher.hpp"

TEST(TaskDispatherTests, TwoPushes) {
    /*std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions{
        {dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}};
    auto pq = std::make_shared<dispatcher::queue::PriorityQueue>(priorityToOptions);
    dispatcher::thread_pool::ThreadPool threadPool{pq, 1};

    int result = 0;
    pq->push(dispatcher::TaskPriority::High, [&result] { result = 1; });

    sleep(2);

    std::cout << "result = " << result << std::endl;*/

    std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions{
        {dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}};

    dispatcher::TaskDispatcher taskDispatcher{2, priorityToOptions};

    taskDispatcher.schedule(dispatcher::TaskPriority::High, [] {});
}