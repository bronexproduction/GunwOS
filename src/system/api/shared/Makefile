CFLAGS=-std=c11 -c -fpic -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -fpic -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)

CFLAGS_SHARED=$(CFLAGS)
CXXFLAGS_SHARED=$(CXXFLAGS)

LFLAGS=-m$(L_OUTFORMAT) -i

O_SHARED_DIR_LISTING=find . -name '*_shared.o' -type f

TARGET_O=gunwapi

.PHONY: all shared

all: shared clean

shared: $(TARGET_O)_shared

$(TARGET_O)_shared: build_shared_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_SHARED_DIR_LISTING))
	$(AR) $(ARFLAGS) lib$@.a $@.o

build_shared_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_SHARED) -o {}_shared.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_SHARED) -o {}_shared.o {}

clean_o:
	find . -type f -name '*.o' ! -name '$(TARGET_O)_shared.o' -delete
	find . -type f -name '*.a' -delete
