#include <gtest/gtest.h>

#include "queue/unbounded_queue.hpp"

TEST(UnboundedQueueTests, TwoPushTwoPop) {
    int capacity = 3;
    dispatcher::queue::UnboundedQueue q;

    std::vector<int> result;

    q.push([&result] { result.push_back(1); });
    q.push([&result] { result.push_back(2); });
    auto task1 = q.try_pop();
    auto task2 = q.try_pop();

    ASSERT_TRUE(task1.has_value());
    ASSERT_TRUE(task2.has_value());

    task1.value()();
    task2.value()();

    EXPECT_EQ(result, (std::vector<int>{1, 2}));
}

TEST(UnboundedQueueTests, PopEmpty) {
    int capacity = 3;
    dispatcher::queue::UnboundedQueue q;

    ASSERT_FALSE(q.try_pop().has_value());
}

TEST(UnboundedQueueTests, Empty) {
    int capacity = 1;
    dispatcher::queue::UnboundedQueue q;

    EXPECT_TRUE(q.empty());
}

// Больше тестов в голову не приходит