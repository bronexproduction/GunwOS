LFLAGS_DEFAULT=-m$(L_OUTFORMAT) -z noexecstack --fatal-warnings -T linker.ld

ifneq ($(GNW_TEST_UNIT),)
KERNEL_TEST_INCLUDE=-include $(TESTS_UNIT_SRC_DIR)/kernel/kernel.pch
LFLAGS=$(LFLAGS_DEFAULT) -L $(KERNEL_BUILD_DIR) -lunittest --no-warn-rwx-segments --allow-multiple-definition
else
KERNEL_TEST_INCLUDE=
LFLAGS=$(LFLAGS_DEFAULT)
endif

KERNEL_INCLUDE=-include kernel.pch \
			   $(KERNEL_TEST_INCLUDE) \
			   -I $(STDGUNW_INCLUDE_DIR) \
			   -I $(APP_API_INCLUDE_DIR) \
			   -I $(DRIVER_API_INCLUDE_DIR) \
			   -I $(API_SHARED_INCLUDE_DIR) \
			   -I $(APP_API_SRC_DIR) \
			   -I $(DRIVER_API_SRC_DIR) \
			   -I . \
			   -I "$(LIB_SRC_DIR)/gunwelf"

KERNEL_LIBS=$(LIB_BUILD_DIR)/stdgunw.o \
			$(LIB_BUILD_DIR)/gunwelf.o \
			$(LIB_BUILD_DIR)/gunwapi_app_kernel.o \
			$(LIB_BUILD_DIR)/gunwapi_driver_kernel.o

export KERNEL_BUILD_CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb -D_GUNWAPI_KERNEL $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)

O_DIR_LISTING=find . -name '*.o' -type f

.PHONY: all unit_tests_bin unit_tests_lib

all: kernel.elf unit_tests_bin clean_o

kernel.elf: kernel.o unit_tests_lib
	$(L) $(LFLAGS) -o $@ $(KERNEL_LIBS) $(shell $(O_DIR_LISTING))
	mv $@ $(KERNEL_BUILD_DIR)/$@

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
