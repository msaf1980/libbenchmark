set(TITLE example)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Mac OS X

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werr" )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # Linux

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pthread -D_REENTRANT -fPIC -lrt" )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set( SOURCES example.c )

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} benchmark)
