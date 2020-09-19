#include <benchmark.h>

#include <unistd.h>

int
benchmark_method(B * b) {
	int i;
	int j;

	/* Do setup here */
	
	/* Start the clock at the last possible second!  */
	b_start_timer(b);

	/* Increment without sample */
	/*for(i = 0; i < b_count(b); i++) {*/
	/* Increment and sample */
	for(i = 0; i < b_count(b); b_sample(b, ++i)) {
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
	BENCH(10000, "bench tests", &benchmark_method);
}

int
main() {
	benchmark_example();

	return BENCH_STATUS;
}

