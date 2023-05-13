ASMFLAGS=-f bin -l boot.lst

all:
	$(ASM) $(ASMFLAGS) -o boot.bin boot.s
	mv boot.bin $(KERNEL_BUILD_DIR)/
	mv boot.lst $(KERNEL_BUILD_DIR)/boot.bin.lst
