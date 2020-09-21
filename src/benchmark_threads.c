/* run threaded benchmark with 2 bench thread */
#include <stddef.h>
#include <stdio.h>

#include "benchmark_internal.h"
#include "benchmark_threads.h"

typedef struct {
    pthread_t tid;
    int status;
    b_bench_method bench_method;
    b_start_barrier *barrier;
    struct B b;
} thread;

static void *thread_bench_start(void *data) {
    thread *t = (thread *) data;
    t->status = t->bench_method(&t->b);
    return NULL;
}

int b_init_barrier(b_start_barrier *barrier, int waiters) {
    if (barrier == NULL) {
        return BENCH_SUCCESS;
    }
    if (waiters < 1) {
        return BENCH_ERROR;
    }
    if (pthread_barrier_init(barrier, NULL, waiters + 1) != 0) {
        return BENCH_ERROR;
    }
    return BENCH_SUCCESS;
}

int b_wait_barrier(b_start_barrier *barrier) {
    int status = pthread_barrier_wait(barrier);
    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
        pthread_barrier_destroy(barrier);
    } else if (status != 0) {
        fprintf(stderr, "error wait barrier with status %d\n", status);
        return BENCH_ERROR;
    }
    return BENCH_SUCCESS;
}

int thread_bench_start_sync(void *b) {
    thread *t = (thread *) ((char *) b - offsetof(thread, b));
    return b_wait_barrier(t->barrier);
}

int update_stats_thread(thread *ts, int threads,
                        struct BenchmarkResult *result) {
    int64_t i;
    int64_t n = 0;
    int64_t count = 0;
    int64_t *sorted;
    double s;
    stat st;
    for (i = 0; i < threads; i++) {
        count += ts[i].b.n;
    }
    result->ns_per_op = 0;
    sorted = malloc(sizeof(int64_t) * count);
    for (i = 0; i < threads; i++) {
        if (ts[i].b.n > 0) {
            int64_t j;
            for (j = 0; j < ts[i].b.n; j++) {
                sorted[j + n] = ts[i].b.samples[j + 1] - ts[i].b.samples[j];
            }
            result->ns_per_op += (double) (ts[i].b.ns_duration / count);
            result->ns_duration += ts[i].b.ns_duration;
            n += ts[i].b.n;
        }
    }
    qsort(sorted, count, sizeof(int64_t), cmpint64p);
    set_stat(&st, sorted, count);

    if (st.median == 0 && threads > 1) {
        for (i = 0; i < threads; i++) {
            sorted[i] = ts[i].b.ns_duration / ts[i].b.n;
        }
        qsort(sorted, threads, sizeof(int64_t), cmpint64p);
        set_stat(&st, sorted, threads);
        result->ns_median = st.median;
        result->ns_min = st.min;
        result->ns_max = st.max;
    }

    result->ns_median = st.median;
    result->ns_min = st.min;
    result->ns_max = st.max;

    result->key = ts[0].b.key;
    result->count = count;

    result->ns_duration /= threads;
    s = (double) (result->ns_duration / NANOS);
    result->ops_per_s = (double) (count / s);

    free(sorted);
    return BENCH_SUCCESS;
}

int b_exec_bench_thread(struct BenchmarkResult *result, int64_t count,
                        benchname_t key, int threads_bench,
                        b_bench_method method_bench, int threads_nobench,
                        b_bench_method method_nobench, void *data,
                        char start_sync) {
    int i;
    int ret = BENCH_SUCCESS;
    thread *ts_nob;
    b_start_barrier barrier;
    b_start_barrier *p_barrier;

    if (start_sync) {
        if (b_init_barrier(&barrier, threads_bench + threads_nobench) !=
            BENCH_SUCCESS) {
            return BENCH_ERROR;
        }
        p_barrier = &barrier;
    } else {
        p_barrier = NULL;
    }

    if (threads_nobench > 0) {
        ts_nob = malloc(sizeof(thread) * threads_nobench);
        for (i = 0; i < threads_nobench; i++) {
            ts_nob[i].b.id = i;
            ts_nob[i].b.key = key;
            ts_nob[i].b.n = count;
            ts_nob[i].b.data = data;
            ts_nob[i].b.running = 0;
            ts_nob[i].b.samples = calloc(sizeof(int64_t), count + 2);
            ts_nob[i].b.start_time.sec = 0;
            ts_nob[i].b.start_time.nsec = 0;
            ts_nob[i].b.end_time.sec = 0;
            ts_nob[i].b.end_time.nsec = 0;

            if (start_sync) {
                ts_nob[i].b.start_sync = thread_bench_start_sync;
            } else {
                ts_nob[i].b.start_sync = NULL;
            }
            ts_nob[i].barrier = p_barrier;

            ts_nob[i].status = BENCH_ERROR;
            ts_nob[i].bench_method = method_nobench;
        }
    } else {
        ts_nob = NULL;
    }

    for (i = 0; i < threads_nobench; i++) {
        if (pthread_create(&ts_nob[i].tid, NULL, thread_bench_start,
                           &ts_nob[i]) != 0) {
            fprintf(stderr, "failed to create thread\n");
            abort();
        }
    }

    ret = b_exec_bench_thread_ex(result, count, key, threads_bench,
                                 method_bench, data, p_barrier);

    if (threads_nobench > 0 && method_nobench != NULL) {
        for (i = 0; i < threads_nobench; i++) {
           ts_nob[i].b.running = 0;
        }
        for (i = 0; i < threads_nobench; i++) {
            pthread_join(ts_nob[i].tid, NULL);
            if (ts_nob[i].status != BENCH_SUCCESS) {
                ret = BENCH_ERROR;
                BENCH_STATUS++;
            }
        }
    }

    free(ts_nob);
    return ret;
}

int b_exec_bench_thread_ex(struct BenchmarkResult *result, int64_t count,
                           benchname_t key, int threads_bench,
                           b_bench_method method_bench, void *data,
                           b_start_barrier *barrier) {
    int i;
    int ret = BENCH_SUCCESS;
    thread *ts;

    if (threads_bench < 1) {
        return BENCH_ERROR;
    }
    ts = malloc(sizeof(thread) * threads_bench);

    result->count = count;
    result->key = key;
    result->threads = threads_bench;

    for (i = 0; i < threads_bench; i++) {
        ts[i].b.id = i;
        ts[i].b.key = key;
        ts[i].b.n = count;
        ts[i].b.data = data;
        ts[i].b.running = 0;
        ts[i].b.samples = calloc(sizeof(int64_t), count + 2);
        ts[i].b.start_time.sec = 0;
        ts[i].b.start_time.nsec = 0;
        ts[i].b.end_time.sec = 0;
        ts[i].b.end_time.nsec = 0;

        if (barrier == NULL) {
            ts[i].b.start_sync = NULL;
        } else {
            ts[i].b.start_sync = thread_bench_start_sync;
        }

        ts[i].status = BENCH_ERROR;
        ts[i].bench_method = method_bench;

        ts[i].barrier = barrier;
    }

    for (i = 0; i < threads_bench; i++) {
        if (pthread_create(&ts[i].tid, NULL, thread_bench_start, &ts[i]) != 0) {
            fprintf(stderr, "failed to create thread\n");
            abort();
        }
    }

    b_wait_barrier(barrier);

    for (i = 0; i < threads_bench; i++) {
        pthread_join(ts[i].tid, NULL);
        if (ts[i].status != BENCH_SUCCESS) {
            ret = BENCH_ERROR;
            BENCH_STATUS++;
        }
    }

    if (ret == BENCH_SUCCESS) {
        update_stats_thread(ts, threads_bench, result);
    }

    for (i = 0; i < threads_bench; i++) {
        free(ts[i].b.samples);
    }

    free(ts);
    return ret;
}
