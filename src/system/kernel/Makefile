LFLAGS_DEFAULT=-m$(L_OUTFORMAT) -pie -z noexecstack --fatal-warnings

ifneq ($(GNW_TEST_UNIT),)
KERNEL_TEST_INCLUDE=-include $(TESTS_UNIT_SRC_DIR)/kernel/kernel.pch
LFLAGS=$(LFLAGS_DEFAULT) -L $(KERNEL_BUILD_DIR) -lunittest --no-warn-rwx-segments --allow-multiple-definition
else
KERNEL_TEST_INCLUDE=
LFLAGS=$(LFLAGS_DEFAULT)
endif

KERNEL_INCLUDE=-include kernel.pch \
			   $(KERNEL_TEST_INCLUDE) \
			   -I $(BOOTLOADER_INCLUDE_DIR) \
			   -I $(STDGUNW_INCLUDE_DIR) \
			   -I $(APP_API_PRV_INCLUDE_DIR) \
			   -I $(APP_API_INCLUDE_DIR) \
			   -I $(DRIVER_API_PRV_INCLUDE_DIR) \
			   -I $(DRIVER_API_INCLUDE_DIR) \
			   -I $(SHARED_API_PRV_INCLUDE_DIR) \
			   -I $(SHARED_API_INCLUDE_DIR) \
			   -I . \
			   -I "$(LIB_SRC_DIR)/gunwelf/include"

KERNEL_LIBS=$(LIB_BUILD_DIR)/stdgunw.o \
			$(LIB_BUILD_DIR)/gunwelf.o \
			$(LIB_BUILD_DIR)/gunwapi_app_kernel.o \
			$(LIB_BUILD_DIR)/gunwapi_driver_kernel.o \
			$(LIB_BUILD_DIR)/gunwapi_shared_kernel.o

export KERNEL_BUILD_CFLAGS=-std=c17 -c -fpic -ffreestanding -mno-red-zone -ggdb -D_GUNWAPI_KERNEL $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++17 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)

O_DIR_LISTING=find . -name '*.o' -type f

.PHONY: all unit_tests_bin unit_tests_lib

all: kernel_paged.elf kernel_paged.ld kernel.elf unit_tests_bin clean_o

kernel_paged.elf: kernel.elf kernel_paged.ld
	$(L) $(LFLAGS) -T $(KERNEL_BUILD_DIR)/kernel_paged.ld -o $(KERNEL_BUILD_DIR)/$@ $(KERNEL_LIBS) $(shell $(O_DIR_LISTING))

kernel_paged.ld:
	cp linker_paged.ld $(KERNEL_BUILD_DIR)/$@
	
kernel.elf: kernel.o unit_tests_lib
	$(L) $(LFLAGS) -T linker.ld -o $(KERNEL_BUILD_DIR)/$@ $(KERNEL_LIBS) $(shell $(O_DIR_LISTING))

kernel.o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(KERNEL_BUILD_CFLAGS) $(KERNEL_INCLUDE) -o {}.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS) -o {}.o {}

unit_tests_bin: kernel.elf
ifneq ($(GNW_TEST_UNIT),)
	make -C $(TESTS_UNIT_SRC_DIR)/kernel bin
endif

unit_tests_lib:
ifneq ($(GNW_TEST_UNIT),)
	make -C $(TESTS_UNIT_SRC_DIR)/kernel lib
endif

clean_o:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete

clean: clean_o
	rm kernel.elf
