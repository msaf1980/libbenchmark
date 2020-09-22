set(TITLE libbenchmark_test)

set(SOURCES test.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
    message(FATAL_ERROR "Cannot find Google Test Framework!")
endif()

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} ${GTEST_BOTH_LIBRARIES} benchmark ${CMAKE_THREAD_LIBS_INIT})
add_test(${TITLE} ${EXECUTABLE_OUTPUT_PATH}/${TITLE})
