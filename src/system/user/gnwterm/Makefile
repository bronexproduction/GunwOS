CFLAGS=$(APP_CFLAGS)
RSFLAGS=$(APP_RSFLAGS)
LFLAGS=$(APP_LFLAGS) -Ttext 0x30000000

O_DIR_LISTING=find . -name '*.o' -type f

TARGET_O=$(shell basename $(shell pwd))

.PHONY: all

all: $(TARGET_O).elf clean_o

$(TARGET_O).elf: build_o
	$(L) $(LFLAGS) -o $@ $(APP_LINK_LIBS) $(shell $(O_DIR_LISTING))
	
build_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS) -o {}.o {}
	$(RS_DIR_LISTING) -print0 | xargs -0I{} $(RUSTC) $(RSFLAGS) -o {}.o {}

clean_o:
	find . -type f -name '*.o' -delete
