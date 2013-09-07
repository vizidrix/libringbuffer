#pragma once
#include <src/ringbuffer.h>

#include <gtest/gtest.h>

class Given_a_ringbuffer_pointer : public testing::Test {
protected:
	rb_ringbuffer * buffer;
};

TEST_F(Given_a_ringbuffer_pointer, _when_a_valid_buffer_is_created) {
	rb_init(&buffer, 4, 1024);

	EXPECT_EQ(4, buffer->ring_size) << "then it should have recorded the correct ring size";
	EXPECT_EQ(1024, buffer->buffer_size) << "then it should have recroded the correct buffer size";
	EXPECT_EQ(0, buffer->write_head) << "then it should have initialized the write head";
	EXPECT_EQ(0, buffer->read_head) << "then it should have initialized the read head";
	EXPECT_EQ(0, buffer->read_tail) << "then it should have initialized the read tail";
}

TEST_F(Given_a_ringbuffer_pointer, _when_a_buffer_is_created_with_invalid_size) {
	rb_init(&buffer, 5, 1024);

	EXPECT_EQ(8, buffer->ring_size) << "then it should have rounded the ring size to the next 2s compliment";
}

TEST_F(Given_a_ringbuffer_pointer, _when_the_buffer_is_closed_twice) {
	rb_init(&buffer, 4, 1024);
	rb_free(&buffer);
	rb_free(&buffer);
}