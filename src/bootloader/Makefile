MODULES=	boot \
			preloader

all: mkbin $(MODULES)

.PHONY: $(MODULES)

$(MODULES):
	make -C $@
	cat $@/$@.bin >> bootloader.bin
	rm $@/$@.bin
	
mkbin:
	touch bootloader.bin
	