#ifndef _BENCHMARK_THREADS_H_
#define _BENCHMARK_THREADS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdint.h>

#include "benchmark.h"

typedef pthread_barrier_t b_start_barrier;

#define BENCH_T(_threads, _barrier, _count, name, method, print_custom, data)  \
    do {                                                                       \
        struct BenchmarkResult bm_result;                                      \
        if (b_exec_bench_thread(&bm_result, _threads, _barrier, _count,        \
                                (benchname_t) name, method,                    \
                                data) == BENCH_SUCCESS) {                      \
            b_print_result(&bm_result, _threads, 1, print_custom, data);       \
        }                                                                      \
    } while (0);

int b_init_barrier(b_start_barrier *barrier, int waiters);
int b_wait_barrier(b_start_barrier *barrier);

int b_exec_bench_thread(struct BenchmarkResult *result, int threads,
                        b_start_barrier *barrier, int64_t count,
                        benchname_t key, b_bench_method bench_method,
                        void *data);

#ifdef __cplusplus
}
#endif

#endif /* _BENCHMARK_THREADS_H_ */
