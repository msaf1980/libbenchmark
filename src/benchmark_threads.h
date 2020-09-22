#ifndef _BENCHMARK_THREADS_H_
#define _BENCHMARK_THREADS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread_barrier.h>
#include <stdint.h>

#include "benchmark.h"

typedef pthread_barrier_t b_start_barrier;

#define BENCH_T(count, name, threads_bench, method_bench, threads_nobench, method_nobench, print_custom,    \
                   data, start_sync)                                              \
    do {                                                                       \
        struct BenchmarkResult bm_result;                                      \
        if (b_exec_bench_thread(&bm_result, count, (benchname_t) name,     \
                                   threads_bench, method_bench,          \
                                    threads_nobench, method_nobench, data,          \
                                   start_sync) == BENCH_SUCCESS) {                \
            b_print_result(&bm_result, threads_bench, 1, print_custom, data);  \
        }                                                                      \
    } while (0);

#define BENCH_T_EX(count, name, threads_bench, method_bench, print_custom,    \
                   data, barrier)                                              \
    do {                                                                       \
        struct BenchmarkResult bm_result;                                      \
        if (b_exec_bench_thread_ex(&bm_result, count, (benchname_t) name,     \
                                   threads_bench, method_bench, data,          \
                                   barrier) == BENCH_SUCCESS) {                \
            b_print_result(&bm_result, threads_bench, 1, print_custom, data);  \
        }                                                                      \
    } while (0);

int b_init_barrier(b_start_barrier *barrier, int waiters);
int b_wait_barrier(b_start_barrier *barrier);

int b_exec_bench_thread(struct BenchmarkResult *result, int64_t count,
                        benchname_t key, int threads_bench,
                        b_bench_method method_bench, int threads_nobench,
                        b_bench_method method_nobench, void *data,
                        char start_sync);

int b_exec_bench_thread_ex(struct BenchmarkResult *result, int64_t count,
                           benchname_t key, int threads_bench,
                           b_bench_method method_bench, void *data,
                           b_start_barrier *barrier);

#ifdef __cplusplus
}
#endif

#endif /* _BENCHMARK_THREADS_H_ */
