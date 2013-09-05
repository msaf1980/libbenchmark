#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#include "benchmark.h"

#define MILLIS 1E6
#define NANOS 1E9

typedef struct nano_clock {
	int64_t	sec;
	int64_t	nsec;
} nano_clock;

struct B {
	const char *				key;
	int							n;
	int 						running;
	void *						clock_token;
	struct nano_clock 			start_time;
	struct nano_clock			end_time;
	uint64_t					s_duration;
	uint64_t					ns_duration;
	b_bench_method		 		bench_method;
};

const char *
b_key(struct B * b) {
	return b->key;
}

int
b_count(struct B * b) {
	return b->n;
}

#ifdef __MACH__

#define GET_TIMESPEC(nc)	get_timespec(nc)

void
get_timespec(nano_clock * nc) {
	nc->nsec = mach_absolute_time();
	nc->sec = nc->nsec / NANOS;
}

#else

#define GET_TIMESPEC(nc)	get_timespec(nc)

void
get_timespec(nano_clock * nc) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	nc->sec = ts.tv_sec;
	nc->nsec = ts.tv_nsec;
}

#endif

int
b_reset_timer(struct B * b) {
	if(b->running) {
		GET_TIMESPEC(&b->start_time);
	}
	b->ns_duration = 0;

	return B_SUCCESS;
}

int
b_start_timer(struct B * b) {
	if(!b->running) {
		GET_TIMESPEC(&b->start_time);
		b->running = 0;
	}

	return B_SUCCESS;
}

int
b_stop_timer(struct B * b) {
	if(b->running) {
		GET_TIMESPEC(&b->end_time);
		if (b->end_time.nsec - b->start_time.nsec < 0) {
			printf("Invalid nsec\n");
		}
		b->s_duration += b->end_time.sec - b->start_time.sec;
		b->ns_duration += b->end_time.nsec - b->start_time.nsec;
		
		b->running = 1;
	}

	return B_SUCCESS;
}

int
b_exec_bench(struct BenchmarkResult * result, int count, const char * key, b_bench_method bench_method) {
	struct B b;
	double ms;
	double s;

	result->count = count;
	result->key = key;

	b.key = key;
	b.n = count;
	b.running = 1;
	b.bench_method = bench_method;

	b_reset_timer(&b);
	b_start_timer(&b);
	b.bench_method(&b);
	b_stop_timer(&b);

	ms = (double)(b.ns_duration / MILLIS);
	s = (double)(b.ns_duration / NANOS);

	result->key = key;
	result->count = count;
	result->ns_per_op = (double)(b.ns_duration / count);
	result->ms_per_op = ms / count;
	result->s_per_op = s / count;
	result->ops_per_s = (double)(count / b.ns_duration);
	result->ops_per_ms = (double)(count / ms);
	result->ops_per_s = (double)(count / s);
	result->s_duration = b.s_duration;
	result->s_mean = 0;
	result->s_median = 0;
	result->s_avg = 0;
	result->ns_duration = b.ns_duration;
	result->ns_mean = 0;
	result->ns_median = 0;
	result->ns_avg = 0;

	return B_SUCCESS;
}

int
b_print_result(struct BenchmarkResult * result) {
	printf("\n%12s\t[%10d]\t[ %8.4f ns/op ]", result->key, result->count, result->ns_per_op);
	printf("\t\t%12.8f op/ms\t\t%12.4f op/s\n\n",
		result->ops_per_ms,
		result->ops_per_s);

	return B_SUCCESS;
}
