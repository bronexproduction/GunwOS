CFLAGS=-std=c11 -c -ffreestanding -Wall -Wextra -ggdb $(CFLAGS_GLOBAL)
LFLAGS=-melf_i386 -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

.PHONY: all build

all: stdgunw.o

stdgunw.o: build
	$(L) $(LFLAGS) -o stdgunw.o $(shell $(O_DIR_LISTING))

build:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
