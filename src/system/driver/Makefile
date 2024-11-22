DRIVER_INCLUDE_PARAMS=-I $(STDGUNW_INCLUDE_DIR) -I $(DRIVER_API_INCLUDE_DIR) -I $(SHARED_API_INCLUDE_DIR)
export DRIVER_CFLAGS=-std=c17 -c -fno-builtin -Wall -Wextra -ggdb $(DRIVER_INCLUDE_PARAMS) $(CFLAGS_GLOBAL)
export DRIVER_LINK_LIBS=$(STDGUNW_LIB) "$(LIB_BUILD_DIR)/gunwapi_driver.o" "$(LIB_BUILD_DIR)/gunwapi_shared.o"
export DRIVER_LFLAGS=-m$(L_OUTFORMAT) --fatal-warnings -z noexecstack -e 0x0
export DRIVER_RSFLAGS=$(RSFLAGS_OBJECT)

DRIVER_DIR_LISTING=find . -mindepth 1 -maxdepth 1 -type d
APPS=$(shell $(DRIVER_DIR_LISTING))

.PHONY: all $(APPS)

all: $(APPS)
	mv *.gdv $(DRIVER_BUILD_DIR)/
	mv *.lst $(DRIVER_BUILD_DIR)/

$(APPS):
	make -C $@
	mv $@/$@.gdv $@.gdv
	objdump -D $@.gdv > $@.lst
