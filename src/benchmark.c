#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
/*#include <iostream>*/
using namespace std;
#endif

#include "benchmark.h"

#define MILLIS 1000000L
#define NANOS 1000000000L

typedef struct nano_clock {
	int64_t	sec;
	int64_t	nsec;
} nano_clock;

struct B {
	char *						key;
	int							n;
	int 						running;
	void *						clock_token;
	struct nano_clock 			start_time;
	struct nano_clock			end_time;
	uint64_t					s_duration;
	uint64_t					ns_duration;
	b_bench_method		 		bench_method;
};

char *
b_key(struct B * b) {
	return b->key;
}

int
b_count(struct B * b) {
	return b->n;
}

#ifdef __MACH__

void
init_clock(struct B * b) {
	clock_serv_t cclock;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	b->clock_token = (void *)&cclock;
}

void
get_timespec(struct B * b, nano_clock * nc) {
	mach_timespec_t mts;
	clock_get_time(*(clock_serv_t *)b->clock_token, &mts);
	nc->sec = mts.tv_sec;
	nc->nsec = mts.tv_nsec;
}

void
free_clock(struct B * b) {
	mach_port_deallocate(mach_task_self(), *(clock_serv_t *)b->clock_token);
}

#else

void
init_clock(struct B * b) {
	b->clock_token = NULL;
}

void
get_timespec(struct B * b, nano_clock * nc) {
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	nc->sec = ts.ts_sec;
	nc->nsec = ts.ts_nsec;
}

void
free_clock(struct B * b) {
	b->clock_token = NULL;
}

#endif

int
b_reset_timer(struct B * b) {
	if(b->running) {
		get_timespec(b, &b->start_time);
	}
	b->ns_duration = 0;

	return B_SUCCESS;
}

int
b_start_timer(struct B * b) {
	if(!b->running) {
		get_timespec(b, &b->end_time);
		b->running = 1;
	}

	return B_SUCCESS;
}

int
b_stop_timer(struct B * b) {
	if(b->running) {
		get_timespec(b, &b->end_time);
		if (b->end_time.nsec - b->start_time.nsec < 0) {
			printf("Invalid nsec\n");
		}
		b->s_duration += b->end_time.sec - b->start_time.sec;
		b->ns_duration += b->end_time.nsec - b->start_time.nsec;
		
		b->running = 0;
	}

	return B_SUCCESS;
}

int
b_exec_bench(struct BenchmarkResult ** result, int count, char * key, b_bench_method bench_method) {
	struct B b;
	b.key = key;
	b.n = count;
	b.running = 0;
	b.bench_method = bench_method;

	init_clock(&b);

	b_reset_timer(&b);
	b_start_timer(&b);
	b.bench_method(&b);
	b_stop_timer(&b);

	free_clock(&b);

	(*result)->key = key;
	(*result)->count = count;
	(*result)->ns_per_op = 0;
	(*result)->ms_per_op = 0;
	(*result)->s_per_op = 0;
	(*result)->s_duration = b.s_duration;
	(*result)->s_mean = 0;
	(*result)->s_median = 0;
	(*result)->s_avg = 0;
	(*result)->ns_duration = b.ns_duration;
	(*result)->ns_mean = 0;
	(*result)->ns_median = 0;
	(*result)->ns_avg = 0;

	return B_SUCCESS;
}

int
b_print_result(struct BenchmarkResult * result) {
	printf("%20s\t[%10d]\t%8.4f ns/op\t\t%12.4f op/ms\t\t%12.4f op/s\n",
		result->key,
		result->count,
		(double)result->ns_per_op / result->count,
		(double)result->ms_per_op / result->count,
		(double)result->s_per_op / result->count);

	return B_SUCCESS;
}
