#include <gtest/gtest.h>
#include <thread>

#include "queue/priority_queue.hpp"
#include <future>

TEST(PriorityQueueTests, TwoPushTwoPop) {
    dispatcher::queue::PriorityQueue pq{
        {{dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}}};

    int result1 = 0;
    int result2 = 0;
    pq.push(dispatcher::TaskPriority::Normal, [&result1] { result1 = 1; });
    pq.push(dispatcher::TaskPriority::Normal, [&result2] { result2 = 2; });

    auto task1 = pq.pop();
    auto task2 = pq.pop();
    ASSERT_TRUE(task1.has_value());
    ASSERT_TRUE(task2.has_value());

    task1.value()();
    task2.value()();

    EXPECT_EQ(result1, 1);
    EXPECT_EQ(result2, 2);
}

TEST(PriorityQueueTests, PriorityCheck) {
    dispatcher::queue::PriorityQueue pq{
        {{dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}}};

    std::vector<int> result_vector;
    pq.push(dispatcher::TaskPriority::Normal, [&result_vector] { result_vector.push_back(1); });
    pq.push(dispatcher::TaskPriority::High, [&result_vector] { result_vector.push_back(2); });
    pq.push(dispatcher::TaskPriority::Normal, [&result_vector] { result_vector.push_back(3); });
    pq.push(dispatcher::TaskPriority::High, [&result_vector] { result_vector.push_back(4); });

    auto task1 = pq.pop();
    auto task2 = pq.pop();
    auto task3 = pq.pop();
    auto task4 = pq.pop();
    ASSERT_TRUE(task1.has_value());
    ASSERT_TRUE(task2.has_value());
    ASSERT_TRUE(task3.has_value());
    ASSERT_TRUE(task4.has_value());

    task1.value()();
    task2.value()();
    task3.value()();
    task4.value()();

    EXPECT_EQ(result_vector, (std::vector<int>{2, 4, 1, 3}));
}

TEST(PriorityQueueTests, MaxCapacityCheck) {
    dispatcher::queue::PriorityQueue pq{
        {{dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}}};

    pq.push(dispatcher::TaskPriority::High, [] {});
    pq.push(dispatcher::TaskPriority::High, [] {});
    ASSERT_THROW(pq.push(dispatcher::TaskPriority::High, [] {}), std::runtime_error);
}

TEST(PriorityQueueTests, WaitOnPop) {
    dispatcher::queue::PriorityQueue pq{
        {{dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}}};

    std::promise<void> promise;
    auto future = promise.get_future();
    int result = 0;

    std::jthread thread{[&]() {
        auto task = pq.pop();
        ASSERT_TRUE(task.has_value());
        task.value()();
        EXPECT_EQ(result, 1);
        promise.set_value();
    }};

    pq.push(dispatcher::TaskPriority::High, [&result] { result = 1; });
    // Спасём себя от вечной блокировки в pop
    if (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
        thread.request_stop();
        FAIL() << "Test timed out";
    }
}

TEST(PriorityQueueTests, CheckShutdown) {
    dispatcher::queue::PriorityQueue pq{
        {{dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}}};

    std::promise<void> promise;
    auto future = promise.get_future();

    std::jthread thread{[&]() {
        auto task = pq.pop();
        ASSERT_TRUE(!task.has_value());
        promise.set_value();
    }};

    pq.shutdown();
    // Спасём себя от вечной блокировки в pop
    if (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
        thread.request_stop();
        FAIL() << "Test timed out";
    }
}