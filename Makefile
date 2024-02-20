# Toolchain versions

GCC_VERSION=9.4.0
BINUTILS_VERSION=2.39
NEWLIB_VERSION=4.2.0.20211231
RUST_VERSION=1.76.0

# Target parameters

TARGET_MACHINE=i386
TARGET_VENDOR=none
TARGET_OS=none
TARGET_BITS=32

# Toolchain directories

# Running via `wsl -e` doesn't set PWD variable
CUR_DIR=$(shell pwd)
TOOLS_DIR="$(CUR_DIR)/tools"
GCC_DIR="$(TOOLS_DIR)/gunwxcc-$(GCC_VERSION)_binutils-$(BINUTILS_VERSION)_newlib-$(NEWLIB_VERSION)"
RUST_DIR="$(TOOLS_DIR)/rust-$(RUST_VERSION)"

# Toolchain executables paths

export ASM=nasm
export C="$(GCC_DIR)/bin/$(TARGET_MACHINE)-elf-gcc"
export CXX="$(GCC_DIR)/bin/$(TARGET_MACHINE)-elf-g++"
export L="$(GCC_DIR)/bin/$(TARGET_MACHINE)-elf-ld"
export AR="$(GCC_DIR)/bin/$(TARGET_MACHINE)-elf-ar"
export RUSTC="$(RUST_DIR)/bin/rustc"

# Output binary params

L_BINFORMAT=elf
export L_OUTFORMAT=$(L_BINFORMAT)_$(TARGET_MACHINE)

# Source directories

SPEC_DIR="$(CUR_DIR)/spec"
SRC_DIR="$(CUR_DIR)/src"
export LIB_SRC_DIR="$(SRC_DIR)/lib"
SYSTEM_SRC_DIR="$(SRC_DIR)/system"
API_DIR="$(SYSTEM_SRC_DIR)/api"
export APP_API_SRC_DIR="$(API_DIR)/app"
export DRIVER_API_SRC_DIR="$(API_DIR)/driver"
export KERNEL_SRC_DIR="$(SYSTEM_SRC_DIR)/kernel"
APPS_SRC_DIR="$(SYSTEM_SRC_DIR)/user"
TESTS_SRC_DIR="$(CUR_DIR)/tests"
TESTS_BLACKBOX_DIR="$(TESTS_SRC_DIR)/blackbox"
TESTS_BLACKBOX_SRC_DIR="$(TESTS_BLACKBOX_DIR)/modules"
TESTS_UNIT_DIR="$(TESTS_SRC_DIR)/unit"
export TESTS_UNIT_SRC_DIR="$(TESTS_UNIT_DIR)/modules"
export SCRIPTS_DIR="$(CUR_DIR)/scripts"
export TEST_BLACKBOX_FRAMEWORK_DIR="$(TESTS_BLACKBOX_DIR)/framework"
export TEST_BLACKBOX_SHARED_DIR="$(TESTS_BLACKBOX_DIR)/shared"

# Header include paths

export STDGUNW_INCLUDE_DIR="$(LIB_SRC_DIR)/stdgunw/include"
export APP_API_INCLUDE_DIR="$(APP_API_SRC_DIR)/include"
export DRIVER_API_INCLUDE_DIR="$(DRIVER_API_SRC_DIR)/include"
export API_SHARED_INCLUDE_DIR="$(API_DIR)/shared"
export API_SYS_DIR="$(API_DIR)/sys"

# Build directories

export BUILD_DIR="$(CUR_DIR)/build"
export LIB_BUILD_DIR="$(BUILD_DIR)/lib"
export KERNEL_BUILD_DIR="$(BUILD_DIR)/kernel"
export APP_BUILD_DIR="$(BUILD_DIR)/app"

# Base library paths

export STDGUNW_LIB="$(LIB_BUILD_DIR)/stdgunw.o"

# Build flags

WARN_PARAMS=-Wall -Wextra -Werror -Wno-error=cpp -Wno-error=unused-parameter
export CFLAGS_GLOBAL=-m$(TARGET_BITS) -fdebug-prefix-map=$(BUILD_DIR)=. $(WARN_PARAMS)
export CXXFLAGS_GLOBAL=$(CFLAGS_GLOBAL)
RSFLAGS_GLOBAL=-g --target=$(SPEC_DIR)/$(TARGET_MACHINE)-$(TARGET_VENDOR)-$(TARGET_OS).json --deny warnings
export RSFLAGS_OBJECT=$(RSFLAGS_GLOBAL) --emit=obj --crate-type=lib
export RSFLAGS_STATICLIB=$(RSFLAGS_GLOBAL) --crate-type=staticlib -Clinker=$(L) -C lto -O \
	-Clink-arg=-L$(GCC_DIR)/lib/gcc/$(TARGET_MACHINE)-$(L_BINFORMAT)/$(GCC_VERSION) \
	-Clink-arg=-lgcc -Clink-arg=-r -Clink-arg=--no-gc-sections 

# Archiver flags

export ARFLAGS=rvs

# Base listing commands

export C_DIR_LISTING=find . -name '*.c' -type f
export CXX_DIR_LISTING=find . -name '*.cpp' -type f
export RS_DIR_LISTING=find . -name '*.rs' -type f

.PHONY: all libs pre_build img clean test_blackbox

all: boot.bin boot.gfb kernel.gfb app_pack

pre_build:
	mkdir -p $(BUILD_DIR) $(KERNEL_BUILD_DIR) $(LIB_BUILD_DIR) $(APP_BUILD_DIR)

boot.bin: pre_build
	make -C $(SRC_DIR)/bootloader/boot

boot.gfb: pre_build
	make -C $(SRC_DIR)/bootloader/preloader

kernel.gfb: kernel.elf
# Remove bytes before .start section
# TO BE IMPROVED - no fixed offset, removing debug data
	dd if="$(KERNEL_BUILD_DIR)/kernel.elf" of="$(KERNEL_BUILD_DIR)/$@" bs=4096 skip=1

kernel.elf: libs api
	make -C $(KERNEL_SRC_DIR)

libs: pre_build
	make -C $(LIB_SRC_DIR)

api: libs
	make -C $(API_DIR)

app_pack: libs api
	make -C $(APPS_SRC_DIR)

img: $(BUILD_DIR)/gunwos.img

$(BUILD_DIR)/gunwos.img:
	bash $(SCRIPTS_DIR)/build_image_fat12.sh $@ $(KERNEL_BUILD_DIR)/boot.bin $(KERNEL_BUILD_DIR)/boot.gfb $(KERNEL_BUILD_DIR)/kernel.gfb $(APP_BUILD_DIR)/*.elf

clean:
	rm -rf $(BUILD_DIR)
	find $(SRC_DIR)/ -type f -name '*.o' -delete
	find $(SRC_DIR)/ -type f -name '*.a' -delete
	find $(SRC_DIR)/ -type f -name '*.elf' -delete
	find $(TESTS_SRC_DIR)/ -type f -name '*.o' -delete

test_blackbox:
	make -C $(TESTS_BLACKBOX_SRC_DIR)/bootloader/boot clean all run
	make -C $(TESTS_BLACKBOX_SRC_DIR)/bootloader/preloader clean all run
	make -C $(TESTS_BLACKBOX_SRC_DIR)/lib clean all run
	make -C $(TESTS_BLACKBOX_SRC_DIR)/api clean all run
	make -C $(TESTS_BLACKBOX_SRC_DIR)/system clean all run
