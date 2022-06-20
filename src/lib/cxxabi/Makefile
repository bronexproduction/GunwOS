CXXFLAGS=-std=c11 -c -ffreestanding -Wall -Wextra -ggdb $(CXXFLAGS_GLOBAL)
LFLAGS=-melf_i386 --trace -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

all: build link

build:
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS) -o {}.o {} $(EXEC_TERMINATOR)

link:
	$(L) $(LFLAGS) -o cxxabi.o $(shell $(O_DIR_LISTING))
