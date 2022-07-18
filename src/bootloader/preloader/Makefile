ASMFLAGS=-f

ASMFLAGS_BOOT=$(ASMFLAGS) bin -l boot.lst

.PHONY: all test

all:
	$(ASM) $(ASMFLAGS_BOOT) -o boot.gfb preloader.s

test:
