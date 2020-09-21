#pragma once
#include <benchmark.h>

#include <gtest/gtest.h>

int last_count = 0;
class Given_a_sample_benchmark_success : public testing::Test {
protected:
	static int sample_success(B * b) {
		printf("sample called: %d", b_count(b));
		last_count = b_count(b);
		return BENCH_SUCCESS;
	}
};

TEST_F(Given_a_sample_benchmark_success, _when_BENCH_is_called) {
	int ret = BENCH_STATUS;
	BENCH(100, "sample success", &sample_success, NULL, NULL)
	EXPECT_EQ(100, last_count) << "then it should have called the sample success";
	EXPECT_EQ(ret, BENCH_STATUS) << "then it should have called the sample success";
}

TEST_F(Given_a_sample_benchmark_success, _when_BENCH_is_called_twice) {
	int pre = BENCH_STATUS;
	BENCH(200, "sample success", &sample_success, NULL, NULL)
	EXPECT_EQ(200, last_count) << "then it should have called the sample success";
	EXPECT_EQ(BENCH_STATUS, pre) << "then it should have called the sample success";
	BENCH(300, "sample success", &sample_success, NULL, NULL)
	EXPECT_EQ(300, last_count) << "then it should have called the sample success";
	EXPECT_EQ(BENCH_STATUS, pre) << "then it should have called the sample success";
}

class Given_a_sample_benchmark_error : public testing::Test {
protected:
	static int sample_error(B * b) {
		printf("sample called: %d", b_count(b));
		last_count = b_count(b);
		return BENCH_ERROR;
	}
};

TEST_F(Given_a_sample_benchmark_error, _when_BENCH_is_called) {
	int pre = BENCH_STATUS;
	BENCH(100, "sample error", &sample_error, NULL, NULL)
	EXPECT_EQ(100, last_count) << "then it should have called the sample error";
	EXPECT_EQ(BENCH_STATUS, pre + 1) << "then it should have called the sample error";
}