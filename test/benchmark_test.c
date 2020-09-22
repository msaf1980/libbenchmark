#include <benchmark.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

static int last_count;
static int sample_success(B *b) {
    printf("sample called: %d", b_count(b));
    last_count = b_count(b);
    return BENCH_SUCCESS;
}

CTEST(sample_benchmark, success_one) {
    int ret = BENCH_STATUS;
    BENCH(100, "sample success", &sample_success, NULL, NULL)
    ASSERT_EQUAL(100, last_count);
    ASSERT_EQUAL(ret, BENCH_STATUS);
}

CTEST(sample_benchmark, success_twice) {
    int pre = BENCH_STATUS;
    BENCH(200, "sample success", &sample_success, NULL, NULL)
    ASSERT_EQUAL(200, last_count);
    ASSERT_EQUAL(BENCH_STATUS, pre);
    BENCH(300, "sample success", &sample_success, NULL, NULL)
    ASSERT_EQUAL(300, last_count);
    ASSERT_EQUAL(BENCH_STATUS, pre);
}

static int sample_error(B *b) {
    printf("sample called: %d", b_count(b));
    last_count = b_count(b);
    return BENCH_ERROR;
}

CTEST(sample_benchmark, error) {
    int pre = BENCH_STATUS;
    BENCH(100, "sample error", &sample_error, NULL, NULL)
    ASSERT_EQUAL(100, last_count);
    ASSERT_EQUAL(BENCH_STATUS, pre + 1);
}

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
