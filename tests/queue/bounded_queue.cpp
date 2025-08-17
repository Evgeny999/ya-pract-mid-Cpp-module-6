#include <gtest/gtest.h>

#include "queue/bounded_queue.hpp"

TEST(BoundedQueueTests, TwoPushTwoPop) {
    int capacity = 3;
    dispatcher::queue::BoundedQueue q{capacity};

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

TEST(BoundedQueueTests, PopEmpty) {
    int capacity = 3;
    dispatcher::queue::BoundedQueue q{capacity};

    ASSERT_FALSE(q.try_pop().has_value());
}

TEST(BoundedQueueTests, MaxCapacityPush) {
    int capacity = 1;
    dispatcher::queue::BoundedQueue q{capacity};

    q.push([] {});
    EXPECT_THROW(q.push([] {}), std::runtime_error);
}

TEST(BoundedQueueTests, Empty) {
    int capacity = 1;
    dispatcher::queue::BoundedQueue q{capacity};
    EXPECT_TRUE(q.empty());
}

TEST(BoundedQueueTests, ZeroCapacityPush) {
    int capacity = 0;
    dispatcher::queue::BoundedQueue q{capacity};
    EXPECT_THROW(q.push([] {}), std::runtime_error);
}