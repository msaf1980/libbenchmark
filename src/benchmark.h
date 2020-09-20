#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

/* Define the success/error codes */
#define BENCH_SUCCESS			0
#define BENCH_ERROR				(-50000)


extern int BENCH_STATUS;

/* Define macros */
#define BENCH(count, name, method)																\
	do {																						\
		struct BenchmarkResult bm_result;														\
		if (b_exec_bench(&bm_result, count, (benchname_t)name, method) == BENCH_SUCCESS) {     	\
			b_print_result(&bm_result);										   			\
		}                                                                                       \
	}while(0);

/* Define the bench struct */
typedef char (* benchname_t)[20];

typedef struct B B;

typedef struct BenchmarkResult {
	benchname_t		key;
	int 			count;
	double			ns_per_op;
	double			ops_per_s;
	int64_t	    	ns_duration;
	/* Statistics */

	double			ns_median;
} BenchmarkResult;

/* Define the public methods */
typedef int (*b_bench_method)(struct B * b);

benchname_t b_key(struct B * b);
int b_count(struct B * b);

void b_sample(struct B * b, int index);
int b_start_timer(struct B * b);
int b_stop_timer(struct B * b);
int b_exec_bench(struct BenchmarkResult * result, int count, benchname_t key, b_bench_method bench_method);
int b_print_result(struct BenchmarkResult * result);

#ifdef __cplusplus
}
#endif

#endif /* _BENCHMARK_H_ */
