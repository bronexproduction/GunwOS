ASMFLAGS=-f elf
CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -Wall -Wextra -ggdb $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -Wall -Wextra -ggdb $(CXXFLAGS_GLOBAL)
LFLAGS=-melf_i386 --trace -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

all: kernel._o build system.o

kernel._o:
	$(ASM) $(ASMFLAGS) -o $@ src/kernel/kernel.s

build:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(C) $(CXXFLAGS) -o {}.o {} $(EXEC_TERMINATOR)

system.o:
	$(L) $(LFLAGS) -o $@ $(shell $(O_DIR_LISTING))
