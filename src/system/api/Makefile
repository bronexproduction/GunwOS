CFLAGS=-std=c11 -c -ffreestanding -mno-red-zone -ggdb -I $(STDGUNW_INCLUDE_DIR) -I $(API_INCLUDE_DIR) $(CFLAGS_GLOBAL)
CXXFLAGS=-std=c++11 -c -ffreestanding -mno-red-zone -ggdb $(CXXFLAGS_GLOBAL)
RSFLAGS=$(RSFLAGS_GLOBAL)
LFLAGS=-m$(L_OUTFORMAT) -i

O_DIR_LISTING=find . -name '*.o' -type f
EXEC_TERMINATOR=\;

TARGET_O=gunwapi

.PHONY: all

all: $(TARGET_O).o clean_o

$(TARGET_O).o: build_o
	$(L) $(LFLAGS) -o $@ $(shell $(O_DIR_LISTING))
	
build_o:
	$(C_DIR_LISTING) -exec $(C) $(CFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(CXX_DIR_LISTING) -exec $(CXX) $(CXXFLAGS) -o {}.o {} $(EXEC_TERMINATOR)
	$(RS_DIR_LISTING) -exec $(RUSTC) $(RSFLAGS) -o {}.o {} $(EXEC_TERMINATOR)

clean_o:
	find . -type f -name '*.o' ! -name '$(TARGET_O).o' -delete
