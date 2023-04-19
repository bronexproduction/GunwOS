APP_INCLUDE_PARAMS=-I $(STDGUNW_INCLUDE_DIR) -I $(APP_API_INCLUDE_DIR)
export APP_CFLAGS=-std=c11 -c -fno-builtin -Wall -Wextra -ggdb $(APP_INCLUDE_PARAMS) $(CFLAGS_GLOBAL)
export APP_LINK_LIBS=$(STDGUNW_LIB) "$(LIB_BUILD_DIR)/gunwapi_app.o"
export APP_LFLAGS=-m$(L_OUTFORMAT) --fatal-warnings -z noexecstack
export APP_RSFLAGS=$(RSFLAGS_GLOBAL)

APP_DIR_LISTING=find . -mindepth 1 -maxdepth 1 -type d
APPS=$(shell $(APP_DIR_LISTING))

.PHONY: all $(APPS)

all: $(APPS)

$(APPS):
	make -C $@
	mv $@/$@.elf $@.elf
