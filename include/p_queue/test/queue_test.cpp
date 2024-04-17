#include <gtest/gtest.h>
#include <iostream>
#include <inttypes.h>

extern "C" {
    #include "lib/p_queue.h"
}

TEST(CREATE_QUEUE, CreateEmptyQueue) {
    p_queue q = create_queue(0, 0);

    EXPECT_TRUE(q.size == 0);
    EXPECT_TRUE(q.first == NULL);
    EXPECT_TRUE(q.last == NULL);

    q_clear(&q);
}

TEST(CREATE_QUEUE, CreateQueueOf5Items) {
    p_queue q = create_queue(5, 0);

    EXPECT_TRUE(q.size == 5);

    item* i = q.first;
    int count = 0;
    while (!(i == NULL)) {
        EXPECT_TRUE(i->data == 0);
        i = i->next;
        ++count;
    }

    EXPECT_TRUE(count == 5);
    q_clear(&q);
}

TEST(IS_EMPTY_QUEUE, IsEmptyQueueOfEmptyQueue) {
    p_queue q = create_queue(0, 0);

    ASSERT_TRUE(is_empty_queue(&q));

    q_clear(&q);
}

TEST(IS_EMPTY_QUEUE, IsEmptyQueueOfNotEmptyQueue) {
    p_queue q = create_queue(5, 0);

    ASSERT_FALSE(is_empty_queue(&q));

    q_clear(&q);
}

TEST(Q_PUSH, PushInEmptyQueue) {
    p_queue q = create_queue(0, 0);

    q_push(&q, 5);

    EXPECT_TRUE(q_continuity(&q));
    EXPECT_TRUE(q.size == 1);
    EXPECT_TRUE(q.last->data == 5);
}

TEST(Q_PUSH, PushInNotEmptyQueue) {
    p_queue q = create_queue(5, 0);

    q_push(&q, 5);

    EXPECT_TRUE(q_continuity(&q));
    EXPECT_TRUE(q.size == 6);
    EXPECT_TRUE(q.last->data == 5);
}

TEST(Q_POP, PopFromQueueOf1Element) {
    p_queue q = create_queue(1, 0);
    q.first->data = 5;

    int64_t data = q_pop(&q);

    EXPECT_TRUE(is_empty_queue(&q));
    EXPECT_TRUE(data == 5);
}

TEST(Q_POP, PopFromQueueOf5Element) {
    p_queue q = create_queue(5, 0);
    q.first->data = 5;

    int64_t data = q_pop(&q);

    EXPECT_TRUE(q_continuity(&q));
    EXPECT_TRUE(q.size == 4);
    EXPECT_TRUE(data == 5);
}

TEST(Q_AT, QueueAtValidIndex) {
    p_queue q = create_queue(5, 5);

    for (int i = 0; i < 5; ++i) {
        int64_t* p = q_at(&q, 1);
        EXPECT_TRUE(!(p == NULL));
        EXPECT_TRUE(*p == 5);
    }
}

TEST(Q_AT, QueueAtInvalidIndex) {
    p_queue q = create_queue(5, 5);

    int64_t* p = q_at(&q, 42);

    ASSERT_TRUE(p == NULL);
}

TEST(Q_COUNT, CountOfQueueOf5Elements) {
    p_queue q = create_queue(5, 5);

    ASSERT_TRUE(q_count(&q) == 5);
}

TEST(Q_CONTINUITY, ContinuityOfContinuiosQueue) {
    p_queue q = create_queue(5, 5);
    ASSERT_TRUE(q_continuity(&q));
}

TEST(Q_CONTINUITY, ContinuityOfEmptyQueue) {
    p_queue q = create_queue(0, 5);
    ASSERT_TRUE(q_continuity(&q));
}

TEST(Q_CONTINUITY, ContinuityOfDisruptedQueue1) {
    p_queue q = create_queue(5, 5);

    item* iterator = q.first;
    for (int i = 1; i < 3; ++i) {
        iterator = iterator->next;
    }

    iterator->next = NULL;

    ASSERT_FALSE(q_continuity(&q));
}

TEST(Q_CONTINUITY, ContinuityOfDisruptedQueue2) {
    p_queue q = create_queue(5, 5);

    q.first = NULL;

    ASSERT_FALSE(q_continuity(&q));
}

TEST(Q_CONTINUITY, ContinuityOfDisruptedQueue3) {
    p_queue q = create_queue(5, 5);

    q.last = NULL;

    ASSERT_TRUE(q_continuity(&q));
}

TEST(Q_VALID, ValidOfContinuiosQueue) {
    p_queue q = create_queue(5, 5);
    ASSERT_TRUE(q_valid(&q));
}

TEST(Q_VALID, ValidOfEmptyQueue) {
    p_queue q = create_queue(0, 5);
    ASSERT_TRUE(q_valid(&q));
}

TEST(Q_VALID, ValidOfDisruptedQueue1) {
    p_queue q = create_queue(5, 5);

    item* iterator = q.first;
    for (int i = 1; i < 3; ++i) {
        iterator = iterator->next;
    }

    iterator->next = NULL;

    ASSERT_FALSE(q_valid(&q));
}

TEST(Q_VALID, ValidOfDisruptedQueue2) {
    p_queue q = create_queue(5, 5);

    q.first = NULL;

    ASSERT_FALSE(q_valid(&q));
}

TEST(Q_VALID, ValidOfDisruptedQueue3) {
    p_queue q = create_queue(5, 5);

    q.last = NULL;

    ASSERT_FALSE(q_valid(&q));
}

TEST(Q_CLEAR, ClearEmptyQueue) {
    p_queue q = create_queue(0, 0);

    EXPECT_TRUE(is_empty_queue(&q));

    q_clear(&q);

    ASSERT_TRUE(is_empty_queue(&q));
}

TEST(Q_CLEAR, ClearNotEmptyQueue) {
    p_queue q = create_queue(5, 0);

    EXPECT_FALSE(is_empty_queue(&q));

    q_clear(&q);

    ASSERT_TRUE(is_empty_queue(&q));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}