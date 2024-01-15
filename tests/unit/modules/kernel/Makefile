LFLAGS=-m$(L_OUTFORMAT) -z noexecstack --fatal-warnings -T linker.ld --no-warn-rwx-segments
O_DIR_LISTING=find . -name '*.o' -type f
C_DIR_LISTING=find . -name '*.c' -type f

TEST_INCLUDE=-I $(KERNEL_SRC_DIR) \
		     -I $(STDGUNW_INCLUDE_DIR)

.PHONY: lib bin kernel_test_unit_build

lib: clean libunittest.a
bin: clean test_unit.gfb

libunittest.a:
	$(RUSTC) $(RSFLAGS_STATICLIB) -o $@ $(TESTS_UNIT_SRC_DIR)/kernel/kernel.rs
	mv $@ $(KERNEL_BUILD_DIR)/$@

test_unit.gfb: test_unit.elf
# Remove bytes before .start section
# TO BE IMPROVED - no fixed offset, removing debug data
	dd if="$(KERNEL_BUILD_DIR)/test_unit.elf" of="$(KERNEL_BUILD_DIR)/$@" bs=4096 skip=1

test_unit.elf: test_unit_build
	$(L) $(LFLAGS) -o $@ $(shell $(O_DIR_LISTING)) --just-symbols=$(KERNEL_BUILD_DIR)/kernel.elf
	mv $@ $(KERNEL_BUILD_DIR)/$@

test_unit_build:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(KERNEL_BUILD_CFLAGS) $(TEST_INCLUDE) -o {}.o {}
	$(RUSTC) $(RSFLAGS_STATICLIB) --test -o $@.o $(TESTS_UNIT_SRC_DIR)/kernel/runner.rs

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete
