ifneq ($(GNW_TEST_UNIT),)
KERNEL_TEST_INCLUDE=-include $(TESTS_UNIT_SRC_DIR)/kernel/kernel.pch
else
KERNEL_TEST_INCLUDE=
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
			$(LIB_BUILD_DIR)/gunwapi_driver_kernel.o \
			$(LIB_BUILD_DIR)/gunwapi_driver.o

CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb $(KERNEL_INCLUDE) -D_GUNWAPI_KERNEL $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_OBJECT)
LFLAGS=-m$(L_OUTFORMAT) -z noexecstack --fatal-warnings -T linker.ld
O_DIR_LISTING=find . -name '*.o' -type f

RSFLAGS_TEST=$(RSFLAGS_STATICLIB) --test
LFLAGS_TEST=$(LFLAGS) -L $(KERNEL_BUILD_DIR) -lunittest --no-warn-rwx-segments
C_TEST_DIR_LISTING=find $(TESTS_UNIT_SRC_DIR)/kernel -name '*.c' -type f
RS_TEST_DIR_LISTING=find $(TESTS_UNIT_SRC_DIR)/kernel -name '*.rs' -type f -depth 1
O_TEST_DIR_LISTING=find $(TESTS_UNIT_SRC_DIR)/kernel -name '*.o' -type f

.PHONY: all

all: kernel.elf clean_o

ifneq ($(GNW_TEST_UNIT),)
kernel.elf: kernel.o libunittest.a
	$(L) $(LFLAGS_TEST) -o $@ $(KERNEL_LIBS) $(shell $(O_DIR_LISTING)) $(shell $(O_TEST_DIR_LISTING))
else
kernel.elf: kernel.o
	$(L) $(LFLAGS) -o $@ $(KERNEL_LIBS) $(shell $(O_DIR_LISTING))
endif
	mv $@ $(KERNEL_BUILD_DIR)/$@

kernel.o:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS) -o {}.o {}
	$(CXX_DIR_LISTING) -print0 | xargs -0I{} $(CXX) $(CXXFLAGS) -o {}.o {}
	$(RS_DIR_LISTING) -print0 | xargs -0I{} $(RUSTC) $(RSFLAGS) -o {}.o {}

libunittest.a:
	$(C_TEST_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS) -o {}.o {}
	$(RUSTC) $(RSFLAGS_TEST) -o $@ $(TESTS_UNIT_SRC_DIR)/kernel/runner.rs
	mv $@ $(KERNEL_BUILD_DIR)/$@

clean_o:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete
	find $(TESTS_UNIT_SRC_DIR)/kernel -type f -name '*.o' -delete
	find $(TESTS_UNIT_SRC_DIR)/kernel -type f -name '*.a' -delete

clean: clean_o
	rm kernel.elf
