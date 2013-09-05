set(TITLE example_c)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Mac OS X

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread -D_REENTRANT -fPIC" )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # Linux

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread -lrt -D_REENTRANT -fPIC" )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(SOURCES example.c)

add_executable(${TITLE} ${SOURCES})
target_link_libraries( ${TITLE} benchmark )