CXXFLAGS=-std=c++11 -c -ffreestanding -Wall -Wextra -ggdb $(CXXFLAGS_GLOBAL)
LFLAGS=-melf_i386 -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

.PHONY: all build

all: cxxabi.o 

cxxabi.o: build
	$(L) $(LFLAGS) -o cxxabi.o $(shell $(O_DIR_LISTING))

build:
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS) -o {}.o {} $(EXEC_TERMINATOR)	
