#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct B B;

typedef struct BenchmarkResult {
	char *		key;
	int 		count;
	uint64_t	ns_per_op;
	uint64_t	ms_per_op;
	uint64_t	s_per_op;
	uint64_t	s_duration;
	uint64_t	s_mean;
	uint64_t	s_median;
	uint64_t	s_avg;
	uint64_t	ns_duration;
	uint64_t	ns_mean;
	uint64_t	ns_median;
	uint64_t	ns_avg;
} BenchmarkResult;

typedef void (*b_bench_method)(struct B * b);

#define B_SUCCESS			0
#define B_ERROR				(-50000)

char * b_key(struct B * b);
int b_count(struct B * b);

int b_reset_timer(struct B * b);
int b_start_timer(struct B * b);
int b_stop_timer(struct B * b);
int b_exec_bench(struct BenchmarkResult ** result, int count, char * key, b_bench_method bench_method);
int b_print_result(struct BenchmarkResult * result);

#ifdef __cplusplus
}
#endif

#endif /* _BENCHMARK_H_ */
