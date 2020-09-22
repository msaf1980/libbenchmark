cmake_minimum_required(VERSION 2.6)

project(LIBBENCHMARK)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(LIBBENCHMARK_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBBENCHMARK_BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIBBENCHMARK_BUILD_ROOT}/lib" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIBBENCHMARK_BUILD_ROOT}/lib" )
set(EXECUTABLE_OUTPUT_PATH         "${LIBBENCHMARK_BUILD_ROOT}/bin" )

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -Werror  -pedantic-errors -Wall -pedantic -Wextra")

include_directories(${LIBBENCHMARK_SRC_ROOT})
add_subdirectory(example)

enable_testing()
add_subdirectory(test)
