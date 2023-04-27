CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) -I $(DRIVER_API_INCLUDE_DIR) -I $(APP_API_INCLUDE_DIR) $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_GLOBAL)

CFLAGS_DRIVER=$(CFLAGS)
CXXFLAGS_DRIVER=$(CXXFLAGS)
RSFLAGS_DRIVER=$(RSFLAGS)

CFLAGS_KERNEL=$(CFLAGS) -D_GUNWAPI_KERNEL
CXXFLAGS_KERNEL=$(CXXFLAGS) -D_GUNWAPI_KERNEL
RSFLAGS_KERNEL=$(RSFLAGS)

LFLAGS=-m$(L_OUTFORMAT) -i

O_DRIVER_DIR_LISTING=find . -name '*_driver.o' -type f
O_KERNEL_DIR_LISTING=find . -name '*_driver_kernel.o' -type f
EXEC_TERMINATOR=\;

TARGET_O=gunwapi

.PHONY: all driver kernel

all: driver kernel clean_o

driver: $(TARGET_O)_driver.o
kernel: $(TARGET_O)_driver_kernel.o

$(TARGET_O)_driver.o: build_driver_o
	$(L) $(LFLAGS) -o $@ $(shell $(O_DRIVER_DIR_LISTING))
	
$(TARGET_O)_driver_kernel.o: build_kernel_o
	$(L) $(LFLAGS) -o $@ $(shell $(O_KERNEL_DIR_LISTING))

build_driver_o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS_DRIVER) -o {}_driver.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS_DRIVER) -o {}_driver.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS_DRIVER) -o {}_driver.o {} $(EXEC_TERMINATOR)

build_kernel_o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS_KERNEL) -o {}_driver_kernel.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS_KERNEL) -o {}_driver_kernel.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS_KERNEL) -o {}_driver_kernel.o {} $(EXEC_TERMINATOR)

clean_o:
	find . -type f -name '*.o' ! -name '$(TARGET_O)_driver.o' -delete ! -name '$(TARGET_O)_driver_kernel.o' -delete