export LIB_CFLAGS=-std=c17 -c -fpic -ffreestanding -Wall -Wextra -ggdb $(CFLAGS_GLOBAL)
export LIB_CXXFLAGS=-std=c++17 -c -fpic -ffreestanding -Wall -Wextra -ggdb $(CXXFLAGS_GLOBAL)
export LIB_LFLAGS=-m$(L_OUTFORMAT) -i

O_DIR_LISTING=find . -maxdepth 1 -name '*.o' -type f

MODULE_DIR_LISTING=find . -mindepth 1 -maxdepth 1 -type d
MODULES=$(shell $(MODULE_DIR_LISTING))

.PHONY: all $(MODULES)

all: $(MODULES) clean

$(MODULES):
ifeq ("$(wildcard $(LIB_BUILD_DIR)/lib$@.a)","")
	make -C $@
	mv $@/$@.o $(LIB_BUILD_DIR)/
	mv $@/lib$@.a $(LIB_BUILD_DIR)/
endif

clean:
	find . -mindepth 2 -type f -name '*.o' -delete
	find . -mindepth 2 -type f -name '*.a' -delete
