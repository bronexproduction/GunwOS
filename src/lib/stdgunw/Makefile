CFLAGS=-std=c11 -c -ffreestanding -Wall -Wextra -ggdb $(CFLAGS_GLOBAL)

LFLAGS=-melf_i386 --trace -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

all: build link

build:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)

link:
	$(L) $(LFLAGS) -o stdgunw.o $(shell $(O_DIR_LISTING))
