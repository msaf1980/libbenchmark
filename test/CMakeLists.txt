set(TITLE benchmark_test)

set(SOURCES benchmark_test.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_executable(${TITLE} ${SOURCES})
#target_link_libraries(${TITLE} ${CMAKE_THREAD_LIBS_INIT})
add_test(${TITLE} ${EXECUTABLE_OUTPUT_PATH}/${TITLE})
