CXX_TESTS_FLAGS=-c -Wall -Wextra

O_DIR_LISTING=find . -name '*.o' -type f

EXEC_TERMINATOR=\;

.PHONY: all run

all:
	$(CXX_DIR_LISTING) -exec g++ $(CXX_TESTS_FLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	ld -o ${BUILD_DIR}/bootloader_tests $(shell $(O_DIR_LISTING))

run:
	${BUILD_DIR}/bootloader_tests
