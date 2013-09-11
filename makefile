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


all: CC_FLAGS		+=	$(RELEASE_OPT)
all: BIN_PATH 		:=	bin/release
all: OBJ_PATH		:=	obj/release
all: O_FILE			:=	$(OBJ_PATH)/$(VNAME).o
all: SO_FILE		:=	$(BIN_PATH)/lib$(VNAME).so
all: release_init make_a


debug: CC_FLAGS		+=	$(DEBUG_OPT)
debug: BIN_PATH		:=	bin/debug
debug: OBJ_PATH		:=	obj/debug
debug: O_FILE		:=	$(OBJ_PATH)/$(VNAME).o
debug: SO_FILE		:=	$(BIN_PATH)/lib$(VNAME).so
debug: debug_init make_a


release_init:
	echo "Making release folders"
	mkdir -p bin/release
	mkdir -p obj/release
	
debug_init:
	echo "Making debug folders"
	mkdir -p bin/debug
	mkdir -p obj/debug

make_o: src/$(NAME).c
	$(CC) $(CC_FLAGS) -fpic -c src/$(NAME).c -Iinc/ -o $(O_FILE)

make_a: make_o
	ar -cvq $(BIN_PATH)/lib$(NAME).a $(OBJ_PATH)/*.o

make_so: make_o
	$(CC) -shared -o $(SO_FILE) $(O_FILE)

example: all
	$(CC) -o bin/release/example example/example.c -Iinc/ -Lbin/release -l$(NAME)

check: all
	$(CC) -o bin/release/$(NAME)_test test/test.cpp -O3 -pthread -D_REENTRANT -Iinc/ -Itest/ -Lbin/release -l$(NAME)

clean:

deepclean:
	# Deep Clean...
	rm -rf bin
	rm -rf obj



#[ -f bin/release/*			 	] && rm -rf bin/release/			|| [ 1 ]


#





# Remove files produced by building batchringbuffer library
	#[ -f obj/batchringbuffer.o 		] && rm -f obj/batchringbuffer.o 		|| [ 1 ]
	#[ -f bin/libbatchringbuffer.a 	] && rm -f bin/libbatchringbuffer.a 	|| [ 1 ]
#example: example.o
#  obj/release/example.o



#example.o: all make_so src/$(NAME).c
#	$(CC) $(CC_FLAGS) -fpic -c example/example.c -o obj/release/example.o

 #-Lbin/release/ -l$(NAME).$(VERSION)
#	$(CC) -o bin/release/example -lbenchmark obj/release/example.o


# 	$(CC) -o example obj/release/$(NAME).$(VERSION).o
#	$(CC) -o example obj/release/benchmark.o


#	$(CC) $(CC_FLAGS) -fpic -c src/$(NAME).c -o $(O_FILE)

#	echo "Building src/$(NAME).c into $(SO_FILE) for release... $(INIT)"
#$(CC) -shared $(SO_FILE) -o $(O_FILE)

#all: $(BIN_PATH)/$(NAME).$(VERSION).so release_init
#: $(OBJ_PATH)/$(NAME).$(VERSION).o

#	$(CC) $(CC_FLAGS) -fpic -c src/$(NAME).c -o $(O_FILE)

#	echo "Building src/$(NAME).c into $(SO_FILE) for debug... $(INIT)"

#$(CC) -shared $(SO_FILE) -o $(O_FILE)

#debug: $(BIN_PATH)/$(NAME).$(VERSION).so debug_init
#: $(OBJ_PATH)/$(NAME).$(VERSION).o



#src/$(NAME).c: $(INIT)
#	echo "src/$(NAME).c"

#$(OBJ_PATH)/$(NAME).$(VERSION).o: src/$(NAME).c
#	$(CC) $(CC_FLAGS) -fpic -c src/$(NAME).c -o $(OBJ_PATH)/$(NAME).$(VERSION).o

#$(SO_FILE): src/$(NAME).c#
#	echo "SO file"

##$(BIN_PATH)/$(NAME).$(VERSION).so: $(OBJ_PATH)/$(NAME).$(VERSION).o
##	$(CC) -shared $(BIN_PATH)/$(NAME).$(VERSION).so -o $(OBJ_PATH)/$(NAME).$(VERSION).o

# example

#.PHONY: all clean check

# Build mode switches in target specific options
#debug: 		bin/debug/$(NAME).$(VERSION).so
#release: 	bin/release/$(NAME).$(VERSION).so

#$(NAME): $(NAME).o
#	$(CC) $(CC_FLAGS) $(OPT) $(SO) src/$(NAME).c -o obj/release/$(NAME).o
#	$(CC) $(CC_FLAGS) $(OPT) -o bin/release/$(NAME).so obj/release/$(NAME).o

#obj/debug/$(NAME).$(VERSION).o: src/$(NAME).c
#	$(CC) $(CC_FLAGS) -fpic -c $< -o $@

#obj/release/$(NAME).$(VERSION).o: src/$(NAME).c
#	$(CC) $(CC_FLAGS) -fpic -c $< -o $@

#bin/debug/$(NAME).$(VERSION).so: obj/debug/$(NAME).$(VERSION).o
#	$(CC) -shared $< -o $@

#bin/release/$(NAME).$(VERSION).so: obj/release/$(NAME).$(VERSION).o
#	$(CC) -shared $< -o $@


#%/.sentinel:
#    $(foreach d,$(subst /, ,$*),mkdir $d && cd $d && ): \
#    touch .sentinel

#$(OBJ_OUT)$(NAME).o: src/$(NAME).c
#	$(CC) -c $< -o $(OBJ_OUT)$@ $(CC_FLAGS)

#$(NAME).so: $(OBJ_OUT)$(NAME).o
#	$(CC) -o $(BIN_OUT)$(NAME).so $(OBJ_OUT)$(NAME).o $(CC_FLAGS)

#$(NAME).so: $(OBJ_OUT)$(NAME).o
#	$(CC) -o $(BIN_OUT)$(NAME).so $(OBJ_OUT)$(NAME).o $(CC_FLAGS)

#example: $(NAME).so
#	gcc example/example .o -o bin/example

#dbg_libbenchmark.so: $(NAME).o
#	$(CC) $(CC_FLAGS)
#main.o: main.c
#	gcc -c main.c

