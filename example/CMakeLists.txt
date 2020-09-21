set(TITLE example)
set(TITLE_THREAD example_thread)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Mac OS X

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werr" )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # Linux

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pthread -D_REENTRANT -fPIC -lrt" )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set( SOURCES example.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c )

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} benchmark)

set( SOURCES_THREAD example_thread.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark_threads.c )
add_executable(${TITLE_THREAD} ${SOURCES_THREAD})
target_link_libraries(${TITLE_THREAD} benchmark Threads::Threads)
