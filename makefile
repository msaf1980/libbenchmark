# ----------------------------------------------------------------
#
#	benchmark makefile
#
#	w/ help from:
#		- http://randu.org/tutorials/c/libraries.php
#		- http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
#
#
# ----------------------------------------------------------------

NAME		=	benchmark
MAJOR		=	0
MINOR		=	1
PATCH		=	0
VERSION 	=	$(MAJOR).$(MINOR).$(PATCH)
VNAME		=	$(NAME).$(VERSION)


# http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
# -O2					// Enables mid level optimizations
# -03					// Enables aggresive optimizations
# -pthread				// Sets preprocessor and linker flags for adding multi-threading support
# -D_REENTRANT			// http://pauillac.inria.fr/~xleroy/linuxthreads/faq.html#H
RELEASE_OPT 		=	-O2
# -D NDEBUG -combine -fwhole-program

# http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html#Debugging-Options
# -g 					// Produces debugging info in native OS format
# -D  eg -DDEBUG		// Enables #ifdef DEBUG check - http://stackoverflow.com/questions/987637/define-debug-1
# -ggdb					// Produces debugging info for use by GDB
# -g3?
DEBUG_OPT			=	-O0 -g -DDEBUG
# -D _DEBUG -g -ggdb -DDEBUG

THREAD 		=	-pthread -D_REENTRANT
# -pedantic-errors		// Many strict ISO C standards
# -Wall					// Adds many compliance check
# -Wextra				// Additional checks beyond Wall
ERR			=	-pedantic-errors -Wall -Wextra -Wno-unused-parameter -Wbad-function-cast

#-march=native
#-ggdb3

# -fPIC					// Makes library address relative to host app for shared objects
SO 			=	-fPIC

# -ansi 				// Conform to ansi c rules
# -std=c99				// Conform to std c plan 99 rules
# -Iinclude				// ...
STRICT 		=	-std=c99 -Iinclude

# -I.					// Includes files from current path
INC_PATH	=	-I.

LIB_PATH	=	

MAC			= -arch i386 -macosx_version_min 10.7

# ----------------------------------------------------------------

CC 			=	gcc

LD			=	ld

# -fPIC 	// Position Independant Code - for shared libs
CC_FLAGS	=	$(STRICT) $(INC_PATH) $(THREAD) $(ERR)

LD_FLAGS	=	$(LIB_PATH) -shared -Wl,-soname,lib$(NAME).so.$(MAJOR) $(MAC)

# ----- TESTS ----------------------------------------------------

LINKING		=	tests/test_link.c

TESTS		=	batchringbuffer_test.c test_link.c

# ----- TARGETS --------------------------------------------------


src/$(NAME).c: makedir inc/$(NAME).h
obj/$(NAME).o: src/$(NAME).c inc/$(NAME).h
	gcc -c src/$(NAME).c -o obj/$(NAME).o -std=c99 -Iinc -pedantic-errors -Wall -Wextra -Wno-unused-parameter -Wbad-function-cast

example/example.c: src/$(NAME).c inc/$(NAME).h
obj/example.o: example/example.c
	gcc -c example/example.c -o obj/example.o -Lbin/release -std=c99 -Iinc -pedantic-errors -Wall -Wextra -Wno-unused-parameter -Wbad-function-cast
example: obj/example.o
	gcc obj/example.o -o bin/release/example -Lbin/release -l$(NAME)

test/test.c: src/$(NAME).c inc/$(NAME).h
obj/test.o: test/test.c
	gcc -c test/test.c -o obj/test.o -Lbin/release -std=c99 -Iinc -pedantic-errors -Wall -Wextra -Wno-unused-parameter -Wbad-function-cast
test: obj/test.o
	gcc obj/test.o -o bin/release/test -Lbin/release -l$(NAME)

bench/bench.c: src/$(NAME).c inc/$(NAME).h
obj/bench.o: bench/bench.c
	gcc -c bench/bench.c -o obj/bench.o -Lbin/release -std=c99 -Iinc -pedantic-errors -Wall -Wextra -Wno-unused-parameter -Wbad-function-cast
bench: obj/bench.o
	gcc obj/bench.o -o bin/release/bench -Lbin/release -l$(NAME)

release: obj/$(NAME).o inc/$(NAME).h
	ar rc bin/release/lib$(NAME).a obj/$(NAME).o

release-all: release example test bench

makedir:
	mkdir -p bin/release
	mkdir -p obj

deepclean:
	rm -rf bin
	rm -rf obj
