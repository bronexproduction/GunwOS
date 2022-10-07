LFLAGS=-melf_i386
LFLAGS_DRY=$(LFLAGS)
LFLAGS_FINAL=$(LFLAGS) -i

O_DIR_LISTING=find . -maxdepth 1 -name '*.o' -type f

MODULES=	stdgunw \
			cxxabi

.PHONY: all $(MODULES)

all: lib.o 

lib.o: $(MODULES) link_dry_run
	$(L) $(LFLAGS_FINAL) -o lib.o $(shell $(O_DIR_LISTING))

$(MODULES):
	make -C $@
	mv $@/$@.o $@.o

# WORKAROUND: Checks if there are any unresolved symbols in the library
# to make sure there are no external dependencies
link_dry_run:
	$(L) $(LFLAGS_DRY) -o lib_dry.o $(shell $(O_DIR_LISTING)) && rm lib_dry.o

