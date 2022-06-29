SRC_DIR=$(PWD)/src
BUILD_DIR=$(PWD)/build
TOOLS_DIR=$(PWD)/tools

export ASM=nasm
export C=$(TOOLS_DIR)/gunwxcc_9.2.0/bin/i386-elf-gcc
export CXX=$(TOOLS_DIR)/gunwxcc_9.2.0/bin/i386-elf-g++
export L=ld
export MAKE=make

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

boot.gfb:
	make -C $(SRC_DIR)/bootloader/preloader
	mv $(SRC_DIR)/bootloader/preloader/boot.gfb $(BUILD_DIR)/$@

kernel.gfb: lib.o
	make -C $(SRC_DIR)/system
	mv $(SRC_DIR)/system/kernel_entry.o $(BUILD_DIR)/
	mv $(SRC_DIR)/system/kernel.o $(BUILD_DIR)/
	$(L) $(LFLAGS_SYSTEM) -o $(BUILD_DIR)/$@ $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/lib.o

lib.o:
	make -C $(SRC_DIR)/lib
	mv $(SRC_DIR)/lib/lib.o $(BUILD_DIR)/$@

img: $(BUILD_DIR)/gunwos.img

$(BUILD_DIR)/gunwos.img:
	cat $(BUILD_DIR)/boot.bin > $@
# COPY >> $(BUILD_DIR)/boot.gfb 
# COPY >> $(BUILD_DIR)/kernel.gfb

clean:
	rm -rf $(BUILD_DIR)
	find $(SRC_DIR)/ -type f -name '*.o' -delete

# $(BUILD_DIR)/kernel.gfb: system_build
# 	$(L) $(LFLAGS_SYSTEM) -o $(BUILD_DIR)/system_raw.bin $(BUILD_DIR)/system/kernel._o $(BUILD_DIR)/system/system.o $(BUILD_DIR)/lib/lib.o 
# 	# Align system binary to 512 bytes
# 	dd if=$(BUILD_DIR)/system_raw.bin of=$(BUILD_DIR)/system_aligned.bin bs=512 conv=sync
# 	# Append 512 bytes terminator at the end
# 	cp $(BUILD_DIR)/system_aligned.bin $@ 
# 	cat $(SRC_DIR)/system/terminator.bin >> $@
# 	echo "System binary size: $$(stat -L -c %s $(BUILD_DIR)/system_raw.bin)"
# 	echo "System aligned binary size: $$(stat -L -c %s $(BUILD_DIR)/system_aligned.bin)"
# 	echo "System terminated binary size: $$(stat -L -c %s $@)"
# 	rm $(BUILD_DIR)/system_raw.bin $(BUILD_DIR)/system_aligned.bin
