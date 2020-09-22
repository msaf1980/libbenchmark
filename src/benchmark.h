#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdlib.h>

/* Define the success/error codes */
#define BENCH_SUCCESS 0
#define BENCH_ERROR (-50000)

#if defined(__GNUC__) && (__GNUC__ >= 4)
#define BENCH_UNUSED_VARIABLE __attribute__((unused))
#else
#define BENCH_UNUSED_VARIABLE
#endif

extern int BENCH_STATUS;

/* Define macros */
#define BENCH(count, name, method, print_custom, data)                         \
    do {                                                                       \
        struct BenchmarkResult bm_result;                                      \
        if (b_exec_bench(&bm_result, count, (benchname_t) name, method,        \
                         data) == BENCH_SUCCESS) {                             \
            b_print_result(&bm_result, 0, 1, print_custom, data);              \
        }                                                                      \
    } while (0);

/* For minimize jitter do repeated benchmarks */
#define BENCH_M(samples, count, name, method, print_custom, data)              \
    do {                                                                       \
        int _i, _n = BENCH_SUCCESS;                                            \
        struct BenchmarkResult bm_result;                                      \
        struct BenchmarkResult *bm_results =                                   \
            malloc(sizeof(BenchmarkResult) * samples);                         \
        for (_i = 0; _i < samples; _i++) {                                     \
            if (b_exec_bench(&bm_results[_i], count, (benchname_t) name,       \
                             method, data) != BENCH_SUCCESS) {                 \
                _n = BENCH_ERROR;                                              \
            }                                                                  \
        }                                                                      \
        if (_n == BENCH_SUCCESS) {                                             \
            _n = b_samples_aggregate(&bm_result, bm_results, samples);         \
            b_print_result(&bm_result, 0, _n, print_custom, data);             \
        }                                                                      \
        free(bm_results);                                                      \
    } while (0);

/* Define the bench struct */
typedef char (*benchname_t)[24];

typedef struct B B;

typedef struct BenchmarkResult {
    benchname_t key;
    int threads;
    int64_t count;
    double ns_per_op;
    double ops_per_s;
    int64_t ns_duration;
    /* Statistics */

    double ns_median;
    double ns_min;
    double ns_max;
} BenchmarkResult;

/* Define the public methods */
typedef int (*b_bench_method)(struct B *b);

typedef void (*b_print_custom_results)(void *data);

benchname_t b_key(struct B *b);
int b_count(struct B *b);

void b_sample(struct B *b, int *index);
int b_running(struct B *b);
int b_start(struct B *b);
int b_start_timer(struct B *b);
int b_stop_timer(struct B *b);
int b_start_sync(struct B *b);
int b_exec_bench(struct BenchmarkResult *result, int64_t count, benchname_t key,
                 b_bench_method bench_method, void *data);

int b_print_header();
int b_print_result(struct BenchmarkResult *result, int threads, int64_t samples,
                   b_print_custom_results print_custom, void *data);
int b_samples_aggregate(struct BenchmarkResult *result,
                        struct BenchmarkResult *s, int64_t samples);

#ifdef __cplusplus
}
#endif

#endif /* _BENCHMARK_H_ */
