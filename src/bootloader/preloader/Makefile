ASMFLAGS=-f bin -l boot.lst

all:
	$(ASM) $(ASMFLAGS) -o boot.gfb preloader.s
	mv boot.gfb $(KERNEL_BUILD_DIR)/
	mv boot.lst $(KERNEL_BUILD_DIR)/boot.gfb.lst
