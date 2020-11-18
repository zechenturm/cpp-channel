//
// Created by moritz on 11/18/20.
//
#include <gtest/gtest.h>

#include "RingBuffer.hpp"

TEST(ringbuffer, a_new_buffer_has_the_capacity_as_specified_in_the_type)
{
    auto buf = RingBuffer<int, 1>{};
    ASSERT_EQ(1, buf.capacity());

    auto buf2 = RingBuffer<int, 10>{};
    ASSERT_EQ(10, buf2.capacity());
}

TEST(ringbuffer, a_new_buffer_is_empty)
{
    auto buf = RingBuffer<int, 1>{};
    ASSERT_TRUE(buf.empty());
}

TEST(ringbuffer, pushing_a_new_element_onto_the_buffer_makes_it_no_longer_empty)
{
    auto buf = RingBuffer<int, 1>{};
    buf.push(15);
    ASSERT_FALSE(buf.empty());
}

TEST(ringbuffer, popping_a_pushed_element_will_yield_that_element)
{
    auto buf = RingBuffer<int, 1>{};
    buf.push(15);
    auto element = buf.pop();
    ASSERT_EQ(15, element);

    buf.push(24);
    element = buf.pop();
    ASSERT_EQ(24, element);
}

TEST(ringbuffer, pushed_elements_are_retrievedd_in_a_first_in_first_out_order)
{
    auto buf = RingBuffer<int, 2>{};
    buf.push(1);
    buf.push(10);
    ASSERT_EQ(1, buf.pop());
    ASSERT_EQ(10, buf.pop());
}

TEST(ringbuffer, a_buffers_size_corresponds_to_the_number_of_elements_it_currently_holds)
{
    auto buf = RingBuffer<int, 2>{};
    ASSERT_EQ(0, buf.size());
    buf.push(9);
    ASSERT_EQ(1, buf.size());
    buf.push(5);
    ASSERT_EQ(2, buf.size());
}

TEST(ringbuffer, a_buffer_is_full_once_the_number_of_elements_it_holds_reaches_its_capacity)
{
    auto buf = RingBuffer<int, 1>{};
    ASSERT_FALSE(buf.full());
    buf.push(1);
    ASSERT_TRUE(buf.full());
}