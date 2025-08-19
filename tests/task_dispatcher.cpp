#include <gtest/gtest.h>

#include "task_dispatcher.hpp"

TEST(TaskDispatherTests, TwoTasksTwoThreads) {
    int result1 = 0;
    int result2 = 0;

    {
        std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions{
            {dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}};

        dispatcher::TaskDispatcher taskDispatcher{2, priorityToOptions};

        taskDispatcher.schedule(dispatcher::TaskPriority::High, [&result1] { result1 = 1; });
        taskDispatcher.schedule(dispatcher::TaskPriority::High, [&result2] { result2 = 2; });
    }

    EXPECT_EQ(result1, 1);
    EXPECT_EQ(result2, 2);
}

TEST(TaskDispatherTests, FourTasksTwoThreads) {
    int result1 = 0;
    int result2 = 0;
    int result3 = 0;
    int result4 = 0;

    {
        std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions{
            {dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}};

        dispatcher::TaskDispatcher taskDispatcher{2, priorityToOptions};

        taskDispatcher.schedule(dispatcher::TaskPriority::High, [&result1] { result1 = 1; });
        taskDispatcher.schedule(dispatcher::TaskPriority::High, [&result2] { result2 = 2; });
        taskDispatcher.schedule(dispatcher::TaskPriority::Normal, [&result3] { result3 = 3; });
        taskDispatcher.schedule(dispatcher::TaskPriority::Normal, [&result4] { result4 = 4; });
    }

    EXPECT_EQ(result1, 1);
    EXPECT_EQ(result2, 2);
    EXPECT_EQ(result3, 3);
    EXPECT_EQ(result4, 4);
}

TEST(TaskDispatherTests, ZeroThreads) {
    std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions{
        {dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, std::nullopt}}};

    ASSERT_THROW((dispatcher::TaskDispatcher{0, priorityToOptions}), std::runtime_error);
}

TEST(TaskDispatherTests, NoHighPriorityQueue) {
    std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions{
        {dispatcher::TaskPriority::Normal, {false, std::nullopt}}};

    dispatcher::TaskDispatcher taskDispatcher{2, priorityToOptions};

    ASSERT_THROW(taskDispatcher.schedule(dispatcher::TaskPriority::High, [] {}), std::runtime_error);
}

TEST(TaskDispatherTests, QueueIsFull) {
    std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions> priorityToOptions{
        {dispatcher::TaskPriority::Normal, {true, 1}}};

    dispatcher::TaskDispatcher taskDispatcher{1, priorityToOptions};

    taskDispatcher.schedule(dispatcher::TaskPriority::Normal, [] {});
    ASSERT_THROW(taskDispatcher.schedule(dispatcher::TaskPriority::Normal, [] {}), std::runtime_error);
}