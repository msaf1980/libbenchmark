set(TITLE example_c)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread -D_REENTRANT -fPIC" )

set(SOURCES example.c)

add_executable(${TITLE} ${SOURCES})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Mac OS X

target_link_libraries( ${TITLE} benchmark )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # Linux

target_link_libraries( ${TITLE} benchmark -lrt )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")