GCC_VERSION=9.4.0
BINUTILS_VERSION=2.39
NEWLIB_VERSION=4.2.0.20211231
RUST_VERSION=1.66.0

SRC_DIR="$(PWD)/src"
LIB_DIR="$(PWD)/src/lib"
TOOLS_DIR="$(PWD)/tools"
SPEC_DIR="$(PWD)/spec"
GCC_DIR="$(TOOLS_DIR)/gunwxcc-$(GCC_VERSION)_binutils-$(BINUTILS_VERSION)_newlib-$(NEWLIB_VERSION)"
RUST_DIR="$(TOOLS_DIR)/rust-$(RUST_VERSION)"
UTL_DIR="$(PWD)/utl"
TESTS_DIR="$(PWD)/tests/modules"
SYSTEM_DIR="$(SRC_DIR)/system"
APPS_DIR="$(SYSTEM_DIR)/src/user"
export SCRIPTS_DIR="$(PWD)/scripts"
export TEST_FRAMEWORK_DIR="$(PWD)/tests/framework"
export TEST_SHARED_DIR="$(PWD)/tests/shared"
export BUILD_DIR="$(PWD)/build"
export STDGUNW_INCLUDE_DIR="$(LIB_DIR)/stdgunw/include"
export API_DIR="$(SYSTEM_DIR)/api"
export API_INCLUDE_DIR="$(API_DIR)/include"
export KERNEL_DIR="$(SYSTEM_DIR)/src/kernel"
export KERNEL_BUILD_DIR="$(BUILD_DIR)/kernel"
export LIB_BUILD_DIR="$(BUILD_DIR)/lib"
export LIB_LIB="$(LIB_BUILD_DIR)/lib.o"
export API_LIB="$(LIB_BUILD_DIR)/gunwapi.o"
export APP_BUILD_DIR="$(BUILD_DIR)/app"

export ASM=nasm
export C="$(GCC_DIR)/bin/i386-elf-gcc"
export CXX="$(GCC_DIR)/bin/i386-elf-g++"
export L="$(GCC_DIR)/bin/i386-elf-ld"
export RUSTC="$(RUST_DIR)/bin/rustc"

WARN_PARAMS=-Wall -Wextra -Werror -Wno-error=cpp -Wno-error=unused-parameter
export CFLAGS_GLOBAL=-fdebug-prefix-map=$(BUILD_DIR)=. $(WARN_PARAMS)
export CXXFLAGS_GLOBAL=$(CFLAGS_GLOBAL)
export RSFLAGS_GLOBAL=--emit=obj --crate-type=lib -g --target=$(SPEC_DIR)/i386-none-none.json

export C_DIR_LISTING=find . -name '*.c' -type f
export CXX_DIR_LISTING=find . -name '*.cpp' -type f
export RS_DIR_LISTING=find . -name '*.rs' -type f

export LFLAGS_KERNEL=-melf_i386 -T $(KERNEL_DIR)/linker.ld

.PHONY: all libs pre_build img clean test 

all: pre_build boot.bin boot.gfb kernel.gfb app_pack

pre_build:
	mkdir -p $(BUILD_DIR)
	mkdir $(KERNEL_BUILD_DIR)
	mkdir $(LIB_BUILD_DIR)
	mkdir $(APP_BUILD_DIR)

boot.bin:
	make -C $(SRC_DIR)/bootloader/boot
	mv $(SRC_DIR)/bootloader/boot/boot.bin $(KERNEL_BUILD_DIR)/$@
	mv $(SRC_DIR)/bootloader/boot/boot.lst $(KERNEL_BUILD_DIR)/$@.lst

boot.gfb:
	make -C $(SRC_DIR)/bootloader/preloader
	mv $(SRC_DIR)/bootloader/preloader/boot.gfb $(KERNEL_BUILD_DIR)/$@
	mv $(SRC_DIR)/bootloader/preloader/boot.lst $(KERNEL_BUILD_DIR)/$@.lst

kernel.gfb: kernel.elf
# Remove bytes before .text section
# TO BE IMPROVED - no fixed offset, removing debug data
	dd if="$(KERNEL_BUILD_DIR)/kernel.elf" of="$(KERNEL_BUILD_DIR)/$@" bs=4096 skip=1

kernel.elf: libs gunwapi.o
	make -C $(KERNEL_DIR)
	mv $(KERNEL_DIR)/$@ $(KERNEL_BUILD_DIR)/$@

libs:
	make -C $(SRC_DIR)/lib
	mv $(SRC_DIR)/lib/*.o $(LIB_BUILD_DIR)/
	
gunwapi.o:
	make -C $(API_DIR)
	mv $(API_DIR)/$@ $(LIB_BUILD_DIR)/$@

app_pack:
	make -C $(APPS_DIR)
	mv $(APPS_DIR)/*.elf $(APP_BUILD_DIR)/

img: $(BUILD_DIR)/gunwos.img

$(BUILD_DIR)/gunwos.img:
	bash $(SCRIPTS_DIR)/build_image_fat12.sh $@ $(KERNEL_BUILD_DIR)/boot.bin $(KERNEL_BUILD_DIR)/boot.gfb $(KERNEL_BUILD_DIR)/kernel.gfb $(APP_BUILD_DIR)/*.elf

clean:
	rm -rf $(BUILD_DIR)
	find $(SRC_DIR)/ -type f -name '*.o' -delete

test:
	make -C $(TESTS_DIR)/bootloader/boot clean all run
	make -C $(TESTS_DIR)/bootloader/preloader clean all run
	make -C $(TESTS_DIR)/lib clean all run
	make -C $(TESTS_DIR)/api clean all run
	make -C $(TESTS_DIR)/system clean all run
