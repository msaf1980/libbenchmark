#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>
#include <float.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#else
#include <unistd.h>
#endif

#ifdef BENCH_THREADS
#include <pthread.h>
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
	int64_t						n;
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
b_sample(struct B * b, int *index) {
	(*index)++;
	b->samples[*index] = get_nanos();
}

int
b_start_timer(struct B * b) {
	if(!b->running) {
		get_timespec(&b->start_time);
		b->samples[0] = b->start_time.sec * NANOS + b->start_time.nsec;
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

typedef struct {
	double median;
	double min;
	double max;	
} stat;

static int cmpint64p(const void *p1, const void *p2) {
  	return ( *(int64_t*)p1 - *(int64_t*)p2 ); 
}

stat
get_stat(int64_t data[], int size) {
    int i = 0;
    stat s;
    int64_t *sorted = malloc(sizeof(int64_t) * size);
    for (i = 0; i < size; i++) {
        sorted[i] = data[i+1] - data[i];
    }
	qsort(sorted, size, sizeof(int64_t), cmpint64p);
    if((size % 2) == 0) {
		s.median = (double)(sorted[size / 2] + sorted[(size / 2) + 1]) / 2;
	} else {
		s.median = (double)sorted[(size / 2) + 1];
	}
	if (s.median == 0) {
		s.min = 0;
		s.max = 0;
	} else {
		s.min = (double)sorted[0];
		s.max = (double)sorted[size-1];
	}
	free(sorted);
	return s;
}

int
update_stats(struct B * b, struct BenchmarkResult * result) {
	stat s = get_stat(b->samples, b->n);
	result->ns_median = s.median;
	result->ns_min = s.min;
	result->ns_max = s.max;

	return BENCH_SUCCESS;
}

static int
cmpdoublep(const void *p1, const void *p2) {
    double d = *((double *) p1) - *((double *) p2);
    if (d > 0) {
        return 1;
    } else if (d < 0) {
        return -1;
    }
    return 0;
}

stat
get_double_stat(double data[], int size) {
    int i = 0;
    stat s;
    double *sorted = malloc(sizeof(double) * size);
    for (i = 0; i < size; i++) {
        sorted[i] = data[i];
    }
	qsort(sorted, size, sizeof(double), cmpdoublep);
    if((size % 2) == 0) {
		s.median = (sorted[size / 2] + sorted[(size / 2) + 1]) / 2;
	} else {
		s.median = sorted[(size / 2) + 1];
	}
	s.min = sorted[0];
	s.max = sorted[size-1];
	free(sorted);
	return s;
}

double
get_double_median(double data[], int size) {
    int i = 0;
	double result;
    double *sorted = malloc(sizeof(double) * size);
    for (i = 0; i < size; i++) {
        sorted[i] = data[i];
    }
	qsort(sorted, size, sizeof(double), cmpdoublep);
    if((size % 2) == 0) {
		result = (sorted[size / 2] + sorted[(size / 2) + 1]) / 2;
	} else {
		result = sorted[(size / 2) + 1];
	}
	free(sorted);
	return result;
}

double
get_double_mean(double data[], int size) {
    int i = 0;
	double result = 0;
	for (i = 0; i < size; i++) {
		result += data[i];
	}
	return result / size;
}

int
b_exec_bench(struct BenchmarkResult * result, int count, benchname_t key, b_bench_method bench_method, void *data) {
	struct B b;
	double s;
	int ret = BENCH_SUCCESS;

	result->count = count;
	result->key = key;

	b.key = key;
	b.n = count;
	b.running = 0;
	b.samples = calloc(sizeof(int64_t), count + 2);
	b.start_time.sec = 0;
	b.start_time.nsec = 0;
	b.end_time.sec = 0;
	b.end_time.nsec = 0;
	b.bench_method = bench_method;

	if (b.bench_method(&b, data) != BENCH_SUCCESS) {
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

const char	* table_header_fmt = "\n%24s\t%6s\t%10s\t%14s\t%14s\t%14s\t%14s\t%14s\t%14s\n";
const char	* table_stats_fmt = "%24s\t%6d\t%10d\t%14.2f\t%14.2f";

const char	* table_nomedian_fmt = "\t%14s\t%14s\t%14s\t%14s\n";
const char	* table_median_fmt = "\t%14.2f\t%14.2f\t%14.2f\t%14.2f\n";


static char first = 0;

int
b_print_result(struct BenchmarkResult * result, int samples) {
	if (samples < 1) {
		return BENCH_ERROR;
	}
	setlocale(LC_NUMERIC, "");
	if (first == 0) {
		first = 1;
		printf(table_header_fmt,
			"test",
			"samples",
			"count",
			"ns/op", "op/s",
			"min ns/op", "max ns/op",			
			"median ns/op", "median op/s"
		);
	}


	printf(table_stats_fmt,
		(char*)result->key,
		samples,
		result->count,
		result->ns_per_op, result->ops_per_s
	);
	if(result->ns_median == 0) {
		printf(table_nomedian_fmt, "-", "-", "-", "-");
	} else {
		printf(table_median_fmt, result->ns_min, result->ns_max, result->ns_median, NANOS / result->ns_median);
	}

	return BENCH_SUCCESS;
}

int
b_samples_aggregate(struct BenchmarkResult * result, struct BenchmarkResult * s, int64_t samples) {
	int i;

	int n = 0;
	double *ns_median_d = malloc(sizeof(double) * samples);
	double *ns_per_op_d = malloc(sizeof(double) * samples);
	double *ops_per_s_d = malloc(sizeof(double) * samples);

	result->count = 0;
	result->key = s[0].key;
	result->ns_duration = 0;
	result->ns_median = 0;
	result->ns_max = DBL_MIN;
	result->ns_min = DBL_MAX;
	result->ns_per_op = 0;
	result->ops_per_s = 0;
	
	for (i  = 0; i < samples; i++) {
		if (s[i].count > 0) {
			result->count += s[i].count;
			result->ns_duration += s[i].ns_duration;
			ns_median_d[n] = s[i].ns_median;
			if (result->ns_max < s[i].ns_max) {
				result->ns_max = s[i].ns_max;
			}
			if (result->ns_min > s[i].ns_min) {
				result->ns_min = s[i].ns_min;
			}
			ns_per_op_d[n] = s[i].ns_per_op;
			ops_per_s_d[n] = s[i].ops_per_s;
			n++;
		}
	}
	if (n > 0) {
		stat st;
		result->ns_duration /= n;
		result->ops_per_s = get_double_mean(ops_per_s_d, n);
		result->ns_per_op = get_double_mean(ns_per_op_d, n);
		st = get_double_stat(ns_per_op_d, n);		
		if (st.median == 0) {
			result->ns_min = 0;
			result->ns_max = 0;
		} else {
			result->ns_median = st.median;
			result->ns_min = st.min;
			result->ns_max = st.max;
		}
	}
	free(ns_median_d);
	free(ns_per_op_d);
	free(ops_per_s_d);
	return n;
}
