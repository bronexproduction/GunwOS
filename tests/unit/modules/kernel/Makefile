LFLAGS=-m$(L_OUTFORMAT) -z noexecstack --fatal-warnings -T linker.ld --no-warn-rwx-segments
O_DIR_LISTING=find . -name '*.o' -type f
C_DIR_LISTING=find . -name '*.c' -type f

TEST_INCLUDE=-I $(KERNEL_SRC_DIR) \
		     -I $(STDGUNW_INCLUDE_DIR)

.PHONY: lib bin kernel_test_unit_build

lib: clean libunittest.a
bin: clean kernel_test_unit.elf

libunittest.a:
	$(RUSTC) $(RSFLAGS_STATICLIB) -o $@ $(TESTS_UNIT_SRC_DIR)/kernel/kernel.rs
	mv $@ $(KERNEL_BUILD_DIR)/$@

kernel_test_unit.elf: kernel_test_unit_build
	$(L) $(LFLAGS) -o $@ $(shell $(O_DIR_LISTING))
	mv $@ $(KERNEL_BUILD_DIR)/$@

kernel_test_unit_build:
	$(C_DIR_LISTING) -print0 | xargs -0I{} $(C) $(KERNEL_BUILD_CFLAGS) $(TEST_INCLUDE) -o {}.o {}
	$(RUSTC) $(RSFLAGS_STATICLIB) --test -o $@.o $(TESTS_UNIT_SRC_DIR)/kernel/runner.rs

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete
