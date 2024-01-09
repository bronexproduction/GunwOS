CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) -I $(APP_API_INCLUDE_DIR) -I $(API_SHARED_INCLUDE_DIR) $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_OBJECT)

CFLAGS_DRIVER=$(CFLAGS)
CXXFLAGS_DRIVER=$(CXXFLAGS)
RSFLAGS_DRIVER=$(RSFLAGS)

CFLAGS_KERNEL=$(CFLAGS) -D_GUNWAPI_KERNEL
CXXFLAGS_KERNEL=$(CXXFLAGS) -D_GUNWAPI_KERNEL
RSFLAGS_KERNEL=$(RSFLAGS)

LFLAGS=-m$(L_OUTFORMAT) -i

O_DRIVER_DIR_LISTING=find . -name '*_driver.o' -type f
O_KERNEL_DIR_LISTING=find . -name '*_driver_kernel.o' -type f

TARGET_O=gunwapi

.PHONY: all driver kernel

all: driver kernel clean

driver: $(TARGET_O)_driver
kernel: $(TARGET_O)_driver_kernel

$(TARGET_O)_driver: build_driver_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_DRIVER_DIR_LISTING))
	$(AR) $(ARFLAGS) lib$@.a $@.o
	
$(TARGET_O)_driver_kernel: build_kernel_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_KERNEL_DIR_LISTING))

build_driver_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_DRIVER) -o {}_driver.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_DRIVER) -o {}_driver.o {}
	$(RS_DIR_LISTING) -print0 | xargs -0I{} $(RUSTC) $(RSFLAGS_DRIVER) -o {}_driver.o {}

build_kernel_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_KERNEL) -o {}_driver_kernel.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_KERNEL) -o {}_driver_kernel.o {}
	$(RS_DIR_LISTING) -print0 | xargs -0I{} $(RUSTC) $(RSFLAGS_KERNEL) -o {}_driver_kernel.o {}

clean_o:
	find . -type f -name '*.o' ! -name '$(TARGET_O)_driver.o' -delete ! -name '$(TARGET_O)_driver_kernel.o' -delete
	find . -type f -name '*.a' -delete
