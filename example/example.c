#include <benchmark.h>

#include <stdio.h>
#include <unistd.h>

int benchmark_sample_method(B *b) {
    int i;
    int j;

    /* Do setup here */

    (void) b_prestart_timer(b);
    /* Start the clock at the last possible second!  */
    (void) b_start_timer(b);

    /* Increment and sample */
    for (i = 0; i < b_count(b); b_sample(b, &i)) {
        /* Do some work here */
        void *p = malloc(4096);
        free(p);
        for (j = 0; j < 1000; j++) {
        }
    }
    /* Stop the clock at the earliest convience */
    b_stop_timer(b);

    /* Do cleanup here */

    return BENCH_SUCCESS;
}

int benchmark_method(B *b) {
    int i;
    int j;

    /* Do setup here */


    (void) b_prestart_timer(b);
    /* Start the clock at the last possible second!  */
    (void) b_start_timer(b);

    /* Increment without sample */
    for (i = 0; i < b_count(b); i++) {
        /* Do some work here */
        void *p = malloc(4096);
        free(p);
        for (j = 0; j < 1000; j++) {
        }
    }
    /* Stop the clock at the earliest convience */
    b_stop_timer(b);

    /* Do cleanup here */

    return BENCH_SUCCESS;
}

void benchmark_sampling_example() {
    BENCH(10000, "bench tests", &benchmark_sample_method, NULL, NULL);
    // For minimize jitter 1000 samples with 10 count
    BENCH_M(1000, 10, "bench samples tests", &benchmark_sample_method, NULL,
            NULL);
}

void benchmark_example() {
    BENCH(10000, "sampling bench tests", &benchmark_method, NULL, NULL);
    // For minimize jitter 1000 samples with 10 count, no sampling for minimal
    // impact
    BENCH_M(1000, 10, "sampling bench samples tests", &benchmark_method, NULL,
            NULL);
}

void custom_print(void *data) {
    const char *msg = (const char *) data;
    printf("\t	%s\t%s\t%s", msg, "test1=value1", "test2=value2");
}

void benchmark_custom_print() {
    // Example for print custom metrics
    const char *msg = "custom";
    BENCH(10000, "bench custom print", &benchmark_sample_method, custom_print,
          (void *) msg);
}

int main() {
    b_print_header();
    benchmark_example();
    benchmark_sampling_example();
    benchmark_custom_print();

    return BENCH_STATUS;
}
