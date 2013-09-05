#include <src/benchmark.h>

#include <stddef.h>
#include <stdio.h>

void
benchmark_method(B * b) {
	int i;

	/* Do setup here */
	b_reset_timer(b);

	for(i = 0; i < b_count(b); i++) {
		/* Do some work here */
	}
	b_stop_timer(b);
	/* Do cleanup here */
}

void
benchmark_example() {
	int result;
	BenchmarkResult * bm_result;

	result = b_exec_bench(&bm_result, 1000, "bench test", &benchmark_method);
	if(result != B_SUCCESS) { return; }
	result = b_print_result(bm_result);
	if(result != B_SUCCESS) { return; }
}

int
main() {
	benchmark_example();

	return 0;
}
