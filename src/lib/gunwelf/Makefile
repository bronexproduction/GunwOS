CFLAGS=$(LIB_CFLAGS) -I $(STDGUNW_INCLUDE_DIR)

O_DIR_LISTING=find . -name '*.o' -type f

TARGET_O=$(shell basename $(shell pwd))

.PHONY: all build

all: $(TARGET_O).o 

$(TARGET_O).o: build
	$(L) $(LIB_LFLAGS) -o $(TARGET_O).o $(shell $(O_DIR_LISTING))
	$(AR) $(ARFLAGS) lib$(TARGET_O).a $(TARGET_O).o

build:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS) -o {}.o {}
