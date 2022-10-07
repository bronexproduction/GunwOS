CXX_FRAMEWORK_FLAGS=-std=c++17 -Wall -Wextra -c
CXX_INCLUDE=-Iinclude

CXX_FRAMEWORK_LISTING=find . -name '*.cpp' -type f
O_FRAMEWORK_LISTING=$(CXX_FRAMEWORK_LISTING) | awk '$$0=$$0".o"'

.PHONY: all prep_env clean

CXX_FRAMEWORK_INPUT=$(shell $(CXX_FRAMEWORK_LISTING))
O_FRAMEWORK_INPUT=$(shell $(O_FRAMEWORK_LISTING))

FRAMEWORK=gunwtest.a

all: prep_env $(BUILD_DIR)/$(FRAMEWORK)

prep_env:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(FRAMEWORK): $(O_FRAMEWORK_INPUT)
	ar rvs $@ $(O_FRAMEWORK_INPUT)
	rm $(O_FRAMEWORK_INPUT)

$(O_FRAMEWORK_INPUT):
	g++ $(CXX_FRAMEWORK_FLAGS) $(CXX_INCLUDE) -o $@ $(shell echo $@ | sed 's/.o$$//')

clean:
	rm -f $(shell $(O_FRAMEWORK_LISTING))
	rm -f $(BUILD_DIR)/$(FRAMEWORK)
