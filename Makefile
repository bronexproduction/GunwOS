SRC_DIR=src
BUILD_DIR=build

export ASM=nasm
export C=$(PWD)/tools/gunwxcc_9.2.0/bin/i386-elf-gcc
export CXX=$(PWD)/tools/gunwxcc_9.2.0/bin/i386-elf-g++
export L=ld
export MAKE=make

export CFLAGS_GLOBAL=-fdebug-prefix-map=$(PWD)/$(BUILD_DIR)=.
export CXXFLAGS_GLOBAL=-fdebug-prefix-map=$(PWD)/$(BUILD_DIR)=.

export C_DIR_LISTING=find . -name '*.c' -type f
export CXX_DIR_LISTING=find . -name '*.cpp' -type f

LFLAGS=-melf_i386 -T linker.ld
LFLAGS_SYSTEM=$(LFLAGS)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

all: $(BUILD_DIR)/system.img

$(BUILD_DIR)/system.img: $(BUILD_DIR)/system.bin
	cat $(BUILD_DIR)/bootloader/bootloader.bin > $@
	cat $(BUILD_DIR)/system.bin >> $@

$(BUILD_DIR)/system.bin: system_build
	$(L) $(LFLAGS_SYSTEM) -o $(BUILD_DIR)/system_raw.bin $(BUILD_DIR)/system/kernel._o $(BUILD_DIR)/system/system.o $(BUILD_DIR)/lib/lib.o 
	# Align system binary to 512 bytes
	dd if=$(BUILD_DIR)/system_raw.bin of=$(BUILD_DIR)/system_aligned.bin bs=512 conv=sync
	# Append 512 bytes terminator at the end
	cp $(BUILD_DIR)/system_aligned.bin $@ 
	cat $(SRC_DIR)/system/terminator.bin >> $@
	echo "System binary size: $$(stat -L -c %s $(BUILD_DIR)/system_raw.bin)"
	echo "System aligned binary size: $$(stat -L -c %s $(BUILD_DIR)/system_aligned.bin)"
	echo "System terminated binary size: $$(stat -L -c %s $@)"
	rm $(BUILD_DIR)/system_raw.bin $(BUILD_DIR)/system_aligned.bin

system_build: $(SRC_DIR)/bootloader/bootloader.bin $(SRC_DIR)/lib/lib.o
	mkdir -p $(BUILD_DIR)/system
	make -C $(SRC_DIR)/system
	mv $(SRC_DIR)/system/system.o $(BUILD_DIR)/system/
	mv $(SRC_DIR)/system/kernel._o $(BUILD_DIR)/system/

$(SRC_DIR)/bootloader/bootloader.bin:
	mkdir -p $(BUILD_DIR)/bootloader
	make -C $(SRC_DIR)/bootloader
	mv $@ $(BUILD_DIR)/bootloader/

$(SRC_DIR)/lib/lib.o:
	mkdir -p $(BUILD_DIR)/lib
	make -C $(SRC_DIR)/lib
	mv $@ $(BUILD_DIR)/lib/
