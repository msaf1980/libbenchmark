add_executable(example example.c)
target_link_libraries(example benchmark)

add_executable(example_thread example_thread.c)
target_link_libraries(example_thread benchmark_threads Threads::Threads)

add_executable(example_thread2 example_thread2.c)
target_link_libraries(example_thread2 benchmark_threads Threads::Threads)

add_executable(example_thread_ex example_thread_ex.c)
target_link_libraries(example_thread_ex benchmark_threads Threads::Threads)

if (MEMSTAT)
    find_library(Mallocstat NAME mallocstat)
    add_executable(example_memstat example.c)
    target_link_libraries(example_memstat benchmarkmem mallocstat)
    target_compile_definitions(example_memstat PUBLIC -DBENCH_MEMSTAT)
endif()