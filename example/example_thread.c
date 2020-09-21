#include <benchmark_threads.h>

#include <unistd.h>
#include <stdio.h>

int
benchmark_sample_method(B * b) {
	int i;
	int j;

	/* Do thread-specific setup here. Also sync bench start */

    if (b_start_sync(b) != BENCH_SUCCESS) {
        return BENCH_ERROR;
    }
	
	/* Start the clock at the last possible second!  */
	b_start_timer(b);

	/* Increment and sample */
	for(i = 0; i < b_count(b); b_sample(b, &i)) {
		/* Do some work here */
		for(j = 0; j < 1000; j++) {
		}
	}
	/* Stop the clock at the earliest convience */
	b_stop_timer(b);

	/* Do cleanup here */

	return BENCH_SUCCESS;
}

int
benchmark_method(B * b) {
	int i;
	int j;

	/* Do thread-specific setup here. Also sync bench start */

    if (b_start_sync(b) != BENCH_SUCCESS) {
        return BENCH_ERROR;
    }
	
	/* Start the clock at the last possible second!  */
	b_start_timer(b);

	/* Increment without sample */
	for(i = 0; i < b_count(b); i++) {
		/* Do some work here */
		for(j = 0; j < 1000; j++) {
		}
	}
	/* Stop the clock at the earliest convience */
	b_stop_timer(b);

	/* Do cleanup here */

	return BENCH_SUCCESS;
}

void
benchmark_example() {
	int threads;
	b_start_barrier barrier;

	threads = 2;
	b_init_barrier(&barrier, threads);
	BENCH_T(threads, &barrier, 1000, "bench samples tests", &benchmark_sample_method, NULL, NULL);
}

void
benchmark_sampling_example() {
	int threads;
	b_start_barrier barrier;

	threads = 2;
	b_init_barrier(&barrier, threads);
	BENCH_T(threads, &barrier, 1000, "sampling bench samples tests", &benchmark_method, NULL, NULL);
}

void
custom_print(void *data) {
	const char *msg = (const char *) data;
	printf("\t	%s\t%s\t%s", msg, "test1=value1", "test2=value2");
}

void
benchmark_custom_print() {
	// Example for print custom metrics
	const char *msg = "custom";

	int threads;
	b_start_barrier barrier;

	threads = 2;
	b_init_barrier(&barrier, threads);
	BENCH_T(threads, &barrier, 1000, "bench custom print", &benchmark_sample_method, custom_print, (void *) msg);
}

int
main() {
    b_print_header();																			\
	benchmark_example();
	benchmark_sampling_example();
	benchmark_custom_print();

	return BENCH_STATUS;
}

