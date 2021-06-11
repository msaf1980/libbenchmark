cmake_minimum_required( VERSION 3.5 )

# Options can be turned on and off by providing cmake with a -DOPTIONNAME=ON (or OFF) argument.
# For example, to turn off benchmarks, and use sanitize flags in a DEBUG build:
#    cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBENCH=OFF -DSANITIZE=ON

# Add test files with filenames <NAME>_test.cpp or <NAME>_test.c into test dir
# Add bench files with filenames <NAME>_bench.cpp or <NAME>_bench.c into test dir

# Override install prefix with CMAKE_INSTALL_PREFIX

include(${CMAKE_SOURCE_DIR}/settings.cmake)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

# Include cmake prescript from cmake dir
file(GLOB cmakeFiles cmake_pre/*.cmake)
foreach(cmakeFile ${cmakeFiles})
	message("INCLUDE ${cmakeFile}")
	INCLUDE(${cmakeFile})
endforeach(cmakeFile)

project( benchmark )

file(GLOB cmakeFiles cmake/*.cmake)
foreach(cmakeFile ${cmakeFiles})
	message("INCLUDE ${cmakeFile}")
	INCLUDE(${cmakeFile})
endforeach(cmakeFile)

if(BENCH)
	set(TEST ON)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories( include )

if(DEFINED ENV{DEVENV})
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

message("ADD src")
add_subdirectory( src )

if (TEST)
	message("ADD example")
	add_subdirectory( example )
	enable_testing()
	#set enable testing
	message("ADD TEST test")
	add_subdirectory( test )
endif() # END TEST

include(${CMAKE_SOURCE_DIR}/options_print.cmake)
