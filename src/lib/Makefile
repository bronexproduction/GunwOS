export LIB_CFLAGS=-std=c11 -c -ffreestanding -Wall -Wextra -ggdb $(CFLAGS_GLOBAL)
export LIB_CXXFLAGS=-std=c++11 -c -ffreestanding -Wall -Wextra -ggdb $(CXXFLAGS_GLOBAL)
export LIB_LFLAGS=-melf_i386 -i

O_DIR_LISTING=find . -maxdepth 1 -name '*.o' -type f

MODULE_DIR_LISTING=find . -mindepth 1 -maxdepth 1 -type d
MODULES=$(shell $(MODULE_DIR_LISTING))

.PHONY: all $(MODULES)

all: $(MODULES) clean_o

$(MODULES):
	make -C $@
	mv $@/$@.o $@.o

clean_o:
	find . -mindepth 2 -type f -name '*.o' -delete
