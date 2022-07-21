CXX_FRAMEWORK_FLAGS=-std=c++17 -Wall -Wextra -c
CXX_INCLUDE=-Iinclude

CXX_FRAMEWORK_LISTING=find . -name '*.cpp' -type f

.PHONY: all prep_env clean

CXX_FRAMEWORK_INPUT=$(shell $(CXX_FRAMEWORK_LISTING))
O_FRAMEWORK_INPUT=$(shell $(CXX_FRAMEWORK_LISTING))

FRAMEWORK=gunwtest.a

all: prep_env $(BUILD_DIR)/$(FRAMEWORK)

prep_env:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(FRAMEWORK): $(O_FRAMEWORK_INPUT)
	ar rvs $@ $(O_FRAMEWORK_INPUT)
	rm $(O_FRAMEWORK_INPUT)

$(O_FRAMEWORK_INPUT):
	g++ $(CXX_FRAMEWORK_FLAGS) $(CXX_INCLUDE) -o $@ $@

clean:
	rm -f $(BUILD_DIR)/$(FRAMEWORK)
