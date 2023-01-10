ASMFLAGS=-f elf
CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -Wall -Wextra -ggdb $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -Wall -Wextra -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_GLOBAL)
LFLAGS=-melf_i386 -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

.PHONY: all

all: kernel.o clean_o

kernel.o: _kernel_entry.o _kernel.o
	$(L) $(LFLAGS) -o $@ $(shell $(O_DIR_LISTING))

_kernel_entry.o:
	$(ASM) $(ASMFLAGS) -o $@ src/kernel/kernel_entry.s

_kernel.o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(C) $(CXXFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	
clean_o:
	find . -type f -name '*.o' ! -name 'kernel.o' -delete
