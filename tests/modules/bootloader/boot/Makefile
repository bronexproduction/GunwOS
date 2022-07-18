CXX_TESTS_FLAGS=-std=c++17 -Wall -Wextra
CXX_INCLUDE=-I$(TEST_FRAMEWORK_DIR)

O_DIR_LISTING=find . -name '*.o' -type f

EXEC_TERMINATOR=\;

.PHONY: all clean run

CXX_INPUT=$(shell $(CXX_DIR_LISTING))

EXECUTABLE=$(BUILD_DIR)/bootloader_tests

all: $(EXECUTABLE)

$(EXECUTABLE):
	g++ -o $@ $(CXX_TESTS_FLAGS) $(CXX_INCLUDE) $(CXX_INPUT)

clean:
	rm -f $(OBJ_INPUT)
	rm -f $(EXECUTABLE)

run:
	$(EXECUTABLE)
