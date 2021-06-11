#ifndef _BENCHMARK_INTERNAL_H_
#define _BENCHMARK_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "benchmark.h"

#ifdef BENCH_MEMSTAT
#include <mallocstat.h>
#endif

#include <stdint.h>

#define MILLIS 1000000
#define NANOS 1000000000

typedef struct nano_clock {
    int64_t sec;
    int64_t nsec;
} nano_clock;

typedef int (*b_sync_method)(void *b /* struct B * b */);

struct B {
    int id;
    benchname_t key;
    int64_t n;
    int running;
    void *clock_token;
    struct nano_clock start_time;
    struct nano_clock end_time;
#ifdef BENCH_MEMSTAT
    alloc_statistic allocstat;
#endif
    int64_t *samples;
    int64_t ns_duration;
    void *data;
    b_sync_method start_sync;
};

int cmpint64p(const void *p1, const void *p2);

typedef struct {
    double median;
    double min;
    double max;
} stat;

void set_stat(stat *s, int64_t sorted[], int64_t size);

#ifdef __cplusplus
}
#endif

#endif /* _BENCHMARK_INTERNAL_H_ */
