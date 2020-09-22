set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -Werror  -pedantic-errors -Wall -pedantic -Wextra")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set( SOURCES example.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c )

set(TITLE example)
add_executable(${TITLE} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c)
target_link_libraries(${TITLE})

set(TITLE_THREAD example_thread)
set( SOURCES_THREAD example_thread.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark_threads.c )
add_executable(${TITLE_THREAD} ${SOURCES_THREAD})
target_link_libraries(${TITLE_THREAD} ${CMAKE_THREAD_LIBS_INIT})

set(TITLE_THREAD2 example_thread2)
set( SOURCES_THREAD2 example_thread2.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark_threads.c )
add_executable(${TITLE_THREAD2} ${SOURCES_THREAD2})
target_link_libraries(${TITLE_THREAD2} ${CMAKE_THREAD_LIBS_INIT})

set(TITLE_THREAD_EX example_thread_ex)
set( SOURCES_THREAD_EX example_thread_ex.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark.c ${CMAKE_CURRENT_SOURCE_DIR}/../src/benchmark_threads.c )
add_executable(${TITLE_THREAD_EX} ${SOURCES_THREAD_EX})
target_link_libraries(${TITLE_THREAD_EX} ${CMAKE_THREAD_LIBS_INIT})
