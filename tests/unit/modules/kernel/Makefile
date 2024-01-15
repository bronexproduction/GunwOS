CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb $(KERNEL_INCLUDE) -D_GUNWAPI_KERNEL $(CFLAGS_GLOBAL)
LFLAGS=-m$(L_OUTFORMAT) -z noexecstack --fatal-warnings -T linker.ld
O_DIR_LISTING=find . -name '*.o' -type f

RSFLAGS=$(RSFLAGS_STATICLIB) --test
LFLAGS_TEST=$(LFLAGS) -L $(KERNEL_BUILD_DIR) -lunittest --allow-multiple-definition --no-warn-rwx-segments
C_TEST_DIR_LISTING=find $(TESTS_UNIT_SRC_DIR)/kernel -name '*.c' -type f
RS_TEST_DIR_LISTING=find $(TESTS_UNIT_SRC_DIR)/kernel -name '*.rs' -type f -depth 1
O_TEST_DIR_LISTING=find $(TESTS_UNIT_SRC_DIR)/kernel -name '*.o' -type f

.PHONY: all

all: clean libunittest.a kernel_test_unit.elf

libunittest.a:
	$(RUSTC) $(RSFLAGS_STATICLIB) -o $@ $(TESTS_UNIT_SRC_DIR)/kernel/kernel.rs
	mv $@ $(KERNEL_BUILD_DIR)/$@

kernel_test_unit.elf:
#	$(C_TEST_DIR_LISTING) -print0 | xargs -0I{} $(C) $(CFLAGS) -o {}.o {}
#	$(RUSTC) $(RSFLAGS_TEST) -o $@ $(TESTS_UNIT_SRC_DIR)/kernel/runner.rs
#	mv $@ $(KERNEL_BUILD_DIR)/$@

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete
