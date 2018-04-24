.POSIX:
.SUFFIXES:

OPT ?= -O3

GCC_ARCH_FLAGS ?= -m64
GPP_ARCH_FLAGS ?= -march=native -m64

# -Wno-deprecated-declarations shuts up Apple OSX clang
FLAGS ?= -Wall -Wno-format -Wno-deprecated-declarations -D_POSIX_C_SOURCE=200112L $(OPT) -DPREFETCH -I. $(CPPFLAGS) -pthread
GPP ?= g++ $(GPP_ARCH_FLAGS) -std=c++11 $(FLAGS)
CFLAGS ?= -Wall -Wno-format -fomit-frame-pointer $(OPT)
GCC ?= gcc $(GCC_ARCH_FLAGS) -std=gnu11 $(CFLAGS)
LIBS ?= -L. -lblake2b

all : 

test_comp:	gtest.cpp Makefile
	$(GPP) -lgtest -lgtest_main -o $@ gtest.cpp sha256.cpp
	
test:	test_comp
	./test_comp
	
lean4:	siphash.h cuckoo.h lean_miner.hpp simple_miner.cpp Makefile
	$(GPP) -o $@ -DSHOW -DIDXSHIFT=0 -DPROOFSIZE=6 -DEDGEBITS=3 simple_miner.cpp $(LIBS)