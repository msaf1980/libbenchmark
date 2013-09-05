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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

void
benchmark_method(B * b) {
	int i;
	/* Do setup here */
	b_reset_timer(b);

	for(i = 0; i < b_count(b); i++) {
		/* Do some work here */
		usleep(1);
	}
	b_stop_timer(b);
	/* Do cleanup here */
}

void
benchmark_example() {
	int result;
	struct BenchmarkResult bm_result;

	result = b_exec_bench(&bm_result, 1000, "bench test", &benchmark_method);
	if(result != B_SUCCESS) { return; }
	result = b_print_result(&bm_result);
	if(result != B_SUCCESS) { return; }
}

int
main() {
	benchmark_example();

	return 0;
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
