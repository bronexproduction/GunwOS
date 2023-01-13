GCC_VERSION=9.4.0
BINUTILS_VERSION=2.39
NEWLIB_VERSION=4.2.0.20211231
RUST_VERSION=1.66.0

SRC_DIR="$(PWD)/src"
GCC_LIB_DIR="$(PWD)/src/lib"
TOOLS_DIR="$(PWD)/tools"
SPEC_DIR="$(PWD)/spec"
GCC_DIR="$(TOOLS_DIR)/gunwxcc-$(GCC_VERSION)_binutils-$(BINUTILS_VERSION)_newlib-$(NEWLIB_VERSION)"
RUST_DIR="$(TOOLS_DIR)/rust-$(RUST_VERSION)"
UTL_DIR="$(PWD)/utl"
TESTS_DIR="$(PWD)/tests/modules"
export SCRIPTS_DIR="$(PWD)/scripts"
export TEST_FRAMEWORK_DIR="$(PWD)/tests/framework"
export TEST_SHARED_DIR="$(PWD)/tests/shared"
export BUILD_DIR="$(PWD)/build"

export ASM=nasm
export C="$(GCC_DIR)/bin/i386-elf-gcc"
export CXX="$(GCC_DIR)/bin/i386-elf-g++"
export L="$(GCC_DIR)/bin/i386-elf-ld"
export RUSTC="$(RUST_DIR)/bin/rustc"

export CFLAGS_GLOBAL=-fdebug-prefix-map=$(BUILD_DIR)=. -I $(GCC_LIB_DIR)
export CXXFLAGS_GLOBAL=-fdebug-prefix-map=$(BUILD_DIR)=.
export RSFLAGS_GLOBAL=--emit=obj --crate-type=lib -g --target=$(SPEC_DIR)/i386-none-none.json

export C_DIR_LISTING=find . -name '*.c' -type f
export CXX_DIR_LISTING=find . -name '*.cpp' -type f
export RS_DIR_LISTING=find . -name '*.rs' -type f

LFLAGS=-melf_i386 -T linker.ld
LFLAGS_SYSTEM=$(LFLAGS)

.PHONY: all pre_build img clean test

all: pre_build boot.bin boot.gfb kernel.gfb

pre_build:
	mkdir -p $(BUILD_DIR)

boot.bin:
	make -C $(SRC_DIR)/bootloader/boot
	mv $(SRC_DIR)/bootloader/boot/boot.bin $(BUILD_DIR)/$@
	mv $(SRC_DIR)/bootloader/boot/boot.lst $(BUILD_DIR)/$@.lst

boot.gfb:
	make -C $(SRC_DIR)/bootloader/preloader
	mv $(SRC_DIR)/bootloader/preloader/boot.gfb $(BUILD_DIR)/$@
	mv $(SRC_DIR)/bootloader/preloader/boot.lst $(BUILD_DIR)/$@.lst

kernel.gfb: kernel.elf
# Remove bytes before .text section
# TO BE IMPROVED - no fixed offset, removing debug data
	dd if="$(BUILD_DIR)/kernel.elf" of="$(BUILD_DIR)/$@" bs=4096 skip=1

kernel.elf: lib.o kernel.o
	$(L) $(LFLAGS_SYSTEM) -o $(BUILD_DIR)/$@ $(BUILD_DIR)/kernel.o $(BUILD_DIR)/lib.o

lib.o:
	make -C $(SRC_DIR)/lib
	mv $(SRC_DIR)/lib/$@ $(BUILD_DIR)/$@

kernel.o:
	make -C $(SRC_DIR)/system
	mv $(SRC_DIR)/system/$@ $(BUILD_DIR)/$@

img: $(BUILD_DIR)/gunwos.img

$(BUILD_DIR)/gunwos.img:
	bash $(SCRIPTS_DIR)/build_image_fat12.sh $@ $(BUILD_DIR)/boot.bin $(BUILD_DIR)/boot.gfb $(BUILD_DIR)/kernel.gfb 

clean:
	rm -rf $(BUILD_DIR)
	find $(SRC_DIR)/ -type f -name '*.o' -delete

test:
	make -C $(TESTS_DIR)/bootloader/boot clean all run
	make -C $(TESTS_DIR)/bootloader/preloader clean all run
	make -C $(TESTS_DIR)/lib clean all run
	make -C $(TESTS_DIR)/system clean all run
