libbenchmark
============

A simple micro-benchmark runner for C

[![Build Status](https://drone.io/github.com/vizidrix/libbenchmark/status.png)](https://drone.io/github.com/vizidrix/libbenchmark/latest)

## Introduction ##

----

After getting hooked on the ease of benchmarking while coding Go I was unable to find a good micro-benchmark utility for C so I whipped this up inside my libringbuffer project then shortly extracted, refactored and pushed this out.

Api is pretty similar to that of the Go benchmark library.

----

Only tested on Mac and Ubuntu so far.

----

## Features ##

> Simple to setup, simple to use
> So far it just returns duration timing
>> Hoping to add things like statistics, outlier detection, averaging, etc some day

## Getting Started ##

Sample code from /example/example.c

```
#include <src/benchmark.h>

#include <unistd.h>

int
benchmark_sample_method(B * b) {
	int i;
	int j;

	/* Do setup here */
	
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

	/* Do setup here */
	
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
benchmark_sampling_example() {
	BENCH(10000, "bench tests", &benchmark_sample_method, NULL, NULL);
	// For minimize jitter 1000 samples with 10 count
	BENCH_S(1000, 10, "bench samples tests", &benchmark_sample_method, NULL, NULL);
}

void
benchmark_example() {
	BENCH(10000, "sampling bench tests", &benchmark_method, NULL, NULL);
	// For minimize jitter 1000 samples with 10 count, no sampling for minimal impact
	BENCH_S(1000, 10, "sampling bench samples tests", &benchmark_method, NULL, NULL);
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
	BENCH(10000, "bench custom print", &benchmark_sample_method, custom_print, (void *) msg);
}

int
main() {
	b_print_header();
	benchmark_example();
	benchmark_sampling_example();
	benchmark_custom_print();

	return BENCH_STATUS;
}
```

----

Version
----
1.0.0 ish

License
----

https://github.com/vizidrix/libbenchmark/blob/master/LICENSE

----
## Edited
* 05-Sept-2013	initial documentation

----
## Credits
* Vizidrix <https://github.com/organizations/vizidrix>
* Perry Birch <https://github.com/PerryBirch>
