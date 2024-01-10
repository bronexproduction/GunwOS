CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) -I $(API_SHARED_INCLUDE_DIR) -I $(API_SYS_DIR)/dispmgr/include -I $(API_SYS_DIR)/kbdmgr/include -I ./include $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_OBJECT)

CFLAGS_APP=$(CFLAGS)
CXXFLAGS_APP=$(CXXFLAGS)
RSFLAGS_APP=$(RSFLAGS)

CFLAGS_KERNEL=$(CFLAGS) -D_GUNWAPI_KERNEL
CXXFLAGS_KERNEL=$(CXXFLAGS) -D_GUNWAPI_KERNEL
RSFLAGS_KERNEL=$(RSFLAGS)

LFLAGS=-m$(L_OUTFORMAT) -i

O_APP_DIR_LISTING=find . -name '*_app.o' -type f
O_KERNEL_DIR_LISTING=find . -name '*_app_kernel.o' -type f

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
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_APP) -o {}_app.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_APP) -o {}_app.o {}
	$(RS_DIR_LISTING) -print0 | xargs -0I{} $(RUSTC) $(RSFLAGS_APP) -o {}_app.o {}

build_kernel_o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS_KERNEL) -o {}_app_kernel.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS_KERNEL) -o {}_app_kernel.o {}
	$(RS_DIR_LISTING) -print0 | xargs -0I{} $(RUSTC) $(RSFLAGS_KERNEL) -o {}_app_kernel.o {}

clean_o:
	find . -type f -name '*.o' ! -name '$(TARGET_O)_app.o' -delete ! -name '$(TARGET_O)_app_kernel.o' -delete
	find . -type f -name '*.a' -delete
