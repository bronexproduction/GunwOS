ASMFLAGS=-f

ASMFLAGS_BOOT=$(ASMFLAGS) bin -l boot.lst

all:
	$(ASM) $(ASMFLAGS_BOOT) -o boot.bin boot.s
