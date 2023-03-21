CFLAGS=$(LIB_CFLAGS) -I $(STDGUNW_INCLUDE_DIR)

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

TARGET_O=$(shell basename $(shell pwd))

.PHONY: all build

all: $(TARGET_O).o 

$(TARGET_O).o: build
	$(L) $(LIB_LFLAGS) -o $(TARGET_O).o $(shell $(O_DIR_LISTING))

build:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)	
