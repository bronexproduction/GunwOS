SRC_DIR=$(PWD)/src
BUILD_DIR=$(PWD)/build
TOOLS_DIR=$(PWD)/tools
UTL_DIR=$(PWD)/utl

export ASM=nasm
export C=$(TOOLS_DIR)/gunwxcc_9.2.0/bin/i386-elf-gcc
export CXX=$(TOOLS_DIR)/gunwxcc_9.2.0/bin/i386-elf-g++
export L=ld

export CFLAGS_GLOBAL=-fdebug-prefix-map=$(BUILD_DIR)=.
export CXXFLAGS_GLOBAL=-fdebug-prefix-map=$(BUILD_DIR)=.

export C_DIR_LISTING=find . -name '*.c' -type f
export CXX_DIR_LISTING=find . -name '*.cpp' -type f

LFLAGS=-melf_i386 -T linker.ld
LFLAGS_SYSTEM=$(LFLAGS)

.PHONY: all pre_build img clean

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

kernel.gfb: lib.o kernel.o
	$(L) $(LFLAGS_SYSTEM) -o $(BUILD_DIR)/$@ $(BUILD_DIR)/kernel.o $(BUILD_DIR)/lib.o

lib.o:
	make -C $(SRC_DIR)/lib
	mv $(SRC_DIR)/lib/$@ $(BUILD_DIR)/$@

kernel.o:
	make -C $(SRC_DIR)/system
	mv $(SRC_DIR)/system/$@ $(BUILD_DIR)/$@

img: $(BUILD_DIR)/gunwos.img

$(BUILD_DIR)/gunwos.img:
	cat $(BUILD_DIR)/boot.bin > $@

clean:
	rm -rf $(BUILD_DIR)
	find $(SRC_DIR)/ -type f -name '*.o' -delete
