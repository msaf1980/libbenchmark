add_executable(test_benchmark benchmark_test.c)
target_link_libraries(test_benchmark benchmark)
add_test(test_benchmark ${EXECUTABLE_OUTPUT_PATH}/test_benchmark)
