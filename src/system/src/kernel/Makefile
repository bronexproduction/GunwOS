KERNEL_INCLUDE=-I $(STDGUNW_INCLUDE_DIR) \
               -I $(API_INCLUDE_DIR) \
			   -I . \
			   -I "$(LIB_SRC_DIR)/gunwelf"

KERNEL_LIBS=$(LIB_BUILD_DIR)/stdgunw.o \
			$(LIB_BUILD_DIR)/gunwapi.o \
			$(LIB_BUILD_DIR)/gunwelf.o

ASMFLAGS=-f elf
CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb $(KERNEL_INCLUDE) $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_GLOBAL)
LFLAGS=-m$(L_OUTFORMAT) -T linker.ld

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

.PHONY: all

all: kernel.elf clean_o

kernel.elf: kernel_entry.o kernel.o
	$(L) $(LFLAGS) -o $@ $(KERNEL_LIBS) $(shell $(O_DIR_LISTING))

kernel_entry.o:
	$(ASM) $(ASMFLAGS) -o $@ kernel_entry.s

kernel.o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	
clean_o:
	find . -type f -name '*.o' -delete

clean: clean_o
	rm kernel.elf
