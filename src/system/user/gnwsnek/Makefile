CFLAGS=$(APP_CFLAGS) -I $(shell pwd)
RSFLAGS=$(APP_RSFLAGS)
LFLAGS=$(APP_LFLAGS) -Ttext=0x00400000

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

TARGET_O=$(shell basename $(shell pwd))

.PHONY: all

all: $(TARGET_O).elf clean_o

$(TARGET_O).elf: build_o
	$(L) $(LFLAGS) -o $@ $(APP_LINK_LIBS) $(shell $(O_DIR_LISTING))
	
build_o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS) -o {}.o {} $(EXEC_TERMINATOR)

clean_o:
	find . -type f -name '*.o' -delete
