ASMFLAGS=-f elf
CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -Wall -Wextra -ggdb $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -Wall -Wextra -ggdb $(CXXFLAGS_GLOBAL)
LFLAGS=-melf_i386 -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

all: kernel_entry.o kernel.o

kernel_entry.o:
	$(ASM) $(ASMFLAGS) -o $@ src/kernel/kernel_entry.s

kernel.o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(C) $(CXXFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(L) $(LFLAGS) -o $@ $(shell $(O_DIR_LISTING))
