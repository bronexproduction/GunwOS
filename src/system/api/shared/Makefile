CFLAGS=-std=c17 -c -fpic -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++17 -c -fpic -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)

CFLAGS_SHARED=$(CFLAGS)
CXXFLAGS_SHARED=$(CXXFLAGS)

CFLAGS_KERNEL=$(CFLAGS) -D_GUNWAPI_KERNEL
CXXFLAGS_KERNEL=$(CXXFLAGS) -D_GUNWAPI_KERNEL

LFLAGS=-m$(L_OUTFORMAT) -i

O_SHARED_DIR_LISTING=find . -name '*_shared.o' -type f
O_KERNEL_DIR_LISTING=find . -name '*_shared_kernel.o' -type f

TARGET_O=gunwapi

.PHONY: all shared kernel

all: shared kernel

shared: $(TARGET_O)_shared
kernel: $(TARGET_O)_shared_kernel

$(TARGET_O)_shared: build_shared_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_SHARED_DIR_LISTING))
	$(AR) $(ARFLAGS) lib$@.a $@.o

$(TARGET_O)_shared_kernel: build_kernel_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_KERNEL_DIR_LISTING))

build_shared_o:
	$(C_SRC_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_SHARED) -o {}_shared.o {}
	$(CXX_SRC_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_SHARED) -o {}_shared.o {}

build_kernel_o:
	$(C_SRC_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_KERNEL) -o {}_shared_kernel.o {}
	$(CXX_SRC_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_KERNEL) -o {}_shared_kernel.o {}
