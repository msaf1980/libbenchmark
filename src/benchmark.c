#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>


#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#else
#include <unistd.h>
#endif

#include "benchmark.h"

#define MILLIS 1E6
#define NANOS 1E9

typedef struct nano_clock {
	int64_t	sec;
	int64_t	nsec;
} nano_clock;


int BENCH_STATUS = 0;

struct B {
	benchname_t					key;
	int							n;
	int 						running;
	void						* clock_token;
	struct nano_clock 			start_time;
	struct nano_clock			end_time;
	int64_t				        * samples;
	int64_t					    ns_duration;
	b_bench_method		 		bench_method;
};

benchname_t
b_get_key(struct B * b) {
	return b->key;
}

int
b_count(struct B * b) {
	return b->n;
}

#ifdef __MACH__

uint64_t
get_nanos() {
	return mach_absolute_time();
}

void
get_timespec(nano_clock * nc) {
	nc->nsec = mach_absolute_time();
	nc->sec = nc->nsec / NANOS;
}

#else

uint64_t
get_nanos() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * NANOS + ts.tv_nsec;
}

void
get_timespec(nano_clock * nc) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	nc->sec = ts.tv_sec;
	nc->nsec = ts.tv_nsec;
}

#endif

inline void
b_sample(struct B * b, int index) {
	b->samples[index] = get_nanos();
}

int
b_start_timer(struct B * b) {
	if(!b->running) {
		get_timespec(&b->start_time);
		b->samples[0] = b->start_time.nsec;
		b->running = 1;
	}

	return BENCH_SUCCESS;
}

int
b_stop_timer(struct B * b) {
	int64_t duration;
	if(b->running) {
		get_timespec(&b->end_time);
		duration = (b->end_time.sec - b->start_time.sec) * NANOS + (b->end_time.nsec - b->start_time.nsec);
		if (duration < 0) {
			printf("Invalid nsec\n");
		}
		b->ns_duration = duration;

		b->running = 0;
	}

	return BENCH_SUCCESS;
}

double
get_median(int64_t data[], int size) {
	int i = 0;
	int j = 0;
	int64_t temp = 0;
	int64_t * sorted;
	double result;
	sorted = malloc(sizeof(int64_t) * (size + 1));
	for (i = 1; i < size+1; i++) {
		sorted[i] = data[i] - data[i-1];
	}
	for (i = size - 1; i > 0; --i) {
		for(j = 0; j < i; ++j) {
			if(sorted[j] > sorted[j+1]) {
				temp = sorted[j];
				sorted[j] = sorted[j+1];
				sorted[j+1] = temp;
			}
		}
	}
	if((size & 1) == 0) {
		result = (double)(sorted[size / 2] + sorted[(size / 2) + 1]) / 2;
	} else {
		result = (double)sorted[(size / 2) + 1];
	}
	free(sorted);
	return result;
}

int
update_stats(struct B * b, struct BenchmarkResult * result) {
	result->ns_median = get_median(&b->samples[0], b->n);

	return BENCH_SUCCESS;
}

int
b_exec_bench(struct BenchmarkResult * result, int count, benchname_t key, b_bench_method bench_method) {
	struct B b;
	double s;
	int ret = BENCH_SUCCESS;

	result->count = count;
	result->key = key;

	b.key = key;
	b.n = count;
	b.running = 0;
	b.samples = calloc(sizeof(uint64_t), count + 2);
	b.start_time.sec = 0;
	b.start_time.nsec = 0;
	b.end_time.sec = 0;
	b.end_time.nsec = 0;
	b.bench_method = bench_method;

	if (b.bench_method(&b) != BENCH_SUCCESS) {
		ret = BENCH_ERROR;
		BENCH_STATUS++;
	}

	s = (double)(b.ns_duration / NANOS);

	result->key = key;
	result->count = count;
	result->ns_per_op = (double)(b.ns_duration / count);
	result->ops_per_s = (double)(count / s);
	result->ns_duration = b.ns_duration;

	update_stats(&b, result);

	free(b.samples);

	return ret;
}

const char	* table_header_fmt = "\n%24s\t%10s\t%14s\t%14s\t%14s\t%14s\n";
const char	* table_stats_fmt = "%24s\t%10d\t%14.2f\t%14.2f";

const char	* table_nomedian_fmt = "\t%14s\t%14s\n";
const char	* table_median_fmt = "\t%14.2f\t%14.2f\n";


static char first = 0;

int
b_print_result(struct BenchmarkResult * result) {
	setlocale(LC_NUMERIC, "");
	if (first == 0) {
		first = 1;
		printf(table_header_fmt,
			"test",
			"count",
			"ns/op", "op/s",
			"median ns/op", "median op/s"
		);
	}


	printf(table_stats_fmt,
		(char*)result->key,
		result->count,
		result->ns_per_op, result->ops_per_s
	);
	if(result->ns_median == 0) {
		printf(table_nomedian_fmt, "-", "-");
	} else {
		printf(table_median_fmt, result->ns_median, NANOS / result->ns_median);
	}

	return BENCH_SUCCESS;
}
