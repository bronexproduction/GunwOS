CFLAGS=$(DRIVER_CFLAGS) -I $(shell pwd)
RSFLAGS=$(DRIVER_RSFLAGS)
LFLAGS=$(DRIVER_LFLAGS) -Ttext 0xA0000000

O_DIR_LISTING=find . -name '*.o' -type f

TARGET_O=$(shell basename $(shell pwd))

.PHONY: all

all: $(TARGET_O).gdv clean_o

$(TARGET_O).gdv: build_o
	$(L) $(LFLAGS) -o $@ $(DRIVER_LINK_LIBS) $(shell $(O_DIR_LISTING))
	
build_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS) -o {}.o {}
	$(RS_DIR_LISTING) -print0 | xargs -0I{} $(RUSTC) $(RSFLAGS) -o {}.o {}

clean_o:
	find . -type f -name '*.o' -delete
