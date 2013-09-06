#include <src/benchmark.h>

#include <unistd.h>

void
benchmark_method(B * b) {
	int i;
	
	/* Do setup here */
	
	/* Increment and optionally sample */
	b_start_timer(b);
	/*for(i = 0; i < b_count(b); i++) {*/
	for(i = 0; i < b_count(b); b_sample(b, ++i)) {
		/* Do some work here */
		usleep(1);
		usleep(1);
		usleep(1);

	}
	b_stop_timer(b);
	/* Do cleanup here */
}

void
benchmark_example() {
	int result;
	struct BenchmarkResult bm_result;

	result = b_exec_bench(&bm_result, 10000, (key_t)"bench tests", &benchmark_method);
	if(result != B_SUCCESS) { return; }
	result = b_print_result(&bm_result);
	if(result != B_SUCCESS) { return; }
}

int
main() {
	benchmark_example();

	return 0;
}
