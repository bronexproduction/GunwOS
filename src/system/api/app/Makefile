CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) -I $(API_SHARED_INCLUDE_DIR) -I $(API_SYS_DIR)/dispmgr/include -I ./include $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_GLOBAL)

CFLAGS_APP=$(CFLAGS)
CXXFLAGS_APP=$(CXXFLAGS)
RSFLAGS_APP=$(RSFLAGS)

CFLAGS_KERNEL=$(CFLAGS) -D_GUNWAPI_KERNEL
CXXFLAGS_KERNEL=$(CXXFLAGS) -D_GUNWAPI_KERNEL
RSFLAGS_KERNEL=$(RSFLAGS)

LFLAGS=-m$(L_OUTFORMAT) -i

O_APP_DIR_LISTING=find . -name '*_app.o' -type f
O_KERNEL_DIR_LISTING=find . -name '*_app_kernel.o' -type f
EXEC_TERMINATOR=\;

TARGET_O=gunwapi

.PHONY: all app kernel

all: app kernel clean_o

app: $(TARGET_O)_app
kernel: $(TARGET_O)_app_kernel

$(TARGET_O)_app: build_app_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_APP_DIR_LISTING))
	$(AR) $(ARFLAGS) lib$@.a $@.o

$(TARGET_O)_app_kernel: build_kernel_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_KERNEL_DIR_LISTING))

build_app_o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS_APP) -o {}_app.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS_APP) -o {}_app.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS_APP) -o {}_app.o {} $(EXEC_TERMINATOR)

build_kernel_o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS_KERNEL) -o {}_app_kernel.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS_KERNEL) -o {}_app_kernel.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS_KERNEL) -o {}_app_kernel.o {} $(EXEC_TERMINATOR)

clean_o:
	find . -type f -name '*.o' ! -name '$(TARGET_O)_app.o' -delete ! -name '$(TARGET_O)_app_kernel.o' -delete
	find . -type f -name '*.a' -delete
