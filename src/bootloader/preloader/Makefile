ASMFLAGS=-f

ASMFLAGS_BOOT=$(ASMFLAGS) bin

all: preloader allocate

preloader:
	$(ASM) $(ASMFLAGS_BOOT) -o preloader_raw.bin preloader.s

allocate:
	dd if=preloader_raw.bin of=preloader.bin bs=512 conv=sync
	rm preloader_raw.bin