#pragma once
#include <src/ringbuffer.h>

#include <gtest/gtest.h>

class Given_a_size_4_ringbuffer : public testing::Test {
protected:
	rb_ringbuffer * buffer;
	int result;

	virtual void SetUp() {
		result = 0;
		rb_init(&buffer, 4, 8);
	}

	virtual void TearDown() {
		result = 0;
		rb_free(&buffer);
	}
};

TEST_F(Given_a_size_4_ringbuffer, _when_a_zero_count_claim_is_requested) {
	uint64_t seq_num = 0;

	result = rb_claim(buffer, &seq_num, 0);

	EXPECT_EQ(RB_COUNT_ZERO, result) << "then it should have returned an error";
}

TEST_F(Given_a_size_4_ringbuffer, _when_a_claim_size_buffer_than_the_buffer_is_requested) {
	uint64_t seq_num = 0;

	result = rb_claim(buffer, &seq_num, 5);

	EXPECT_EQ(RB_COUNT_OVERFLOW, result) << "then it should have returned a count overflow error";
}

TEST_F(Given_a_size_4_ringbuffer, _when_a_valid_single_claim_is_requested) {
	uint64_t seq_num = 0;

	result = rb_claim(buffer, &seq_num, 1);

	EXPECT_EQ(RB_SUCCESS, result) << "then it should have returned a success";
	EXPECT_EQ(0, seq_num) << "then the seq_num should have been zero for the first entry";
}

TEST_F(Given_a_size_4_ringbuffer, _when_two_valid_claims_are_made) {
	uint64_t seq_num1 = 0;
	uint64_t seq_num2 = 0;

	rb_claim(buffer, &seq_num1, 1);
	result = rb_claim(buffer, &seq_num2, 1);

	EXPECT_EQ(RB_SUCCESS, result) << "then the second claim should have returned a success";
	EXPECT_EQ(0, seq_num1) << "then the first seq_num should have been 0";
	EXPECT_EQ(1, seq_num2) << "then the second sec_num should have been 1";
}
