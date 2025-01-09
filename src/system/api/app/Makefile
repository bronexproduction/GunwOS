CFLAGS=-std=c17 -c -fpic -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) -I $(SHARED_API_PRV_INCLUDE_DIR) -I $(SHARED_API_INCLUDE_DIR) -I $(API_SYS_DIR)/dispmgr/include -I $(API_SYS_DIR)/kbdmgr/include -I ./include $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++17 -c -fpic -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)

CFLAGS_APP=$(CFLAGS)
CXXFLAGS_APP=$(CXXFLAGS)

CFLAGS_KERNEL=$(CFLAGS) -D_GUNWAPI_KERNEL
CXXFLAGS_KERNEL=$(CXXFLAGS) -D_GUNWAPI_KERNEL

LFLAGS=-m$(L_OUTFORMAT) -i

O_APP_DIR_LISTING=find . -name '*_app.o' -type f
O_KERNEL_DIR_LISTING=find . -name '*_app_kernel.o' -type f

TARGET_O=gunwapi

.PHONY: all app kernel

all: app kernel

app: $(TARGET_O)_app
kernel: $(TARGET_O)_app_kernel

$(TARGET_O)_app: build_app_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_APP_DIR_LISTING))
	$(AR) $(ARFLAGS) lib$@.a $@.o

$(TARGET_O)_app_kernel: build_kernel_o
	$(L) $(LFLAGS) -o $@.o $(shell $(O_KERNEL_DIR_LISTING))

build_app_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_APP) -o {}_app.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_APP) -o {}_app.o {}

build_kernel_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_KERNEL) -o {}_app_kernel.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_KERNEL) -o {}_app_kernel.o {}
