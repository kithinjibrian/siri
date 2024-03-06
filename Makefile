CC = x86_64-w64-mingw32-g++
CFLAGS = -I../fetch/include -I../loader/include -s -O2 -g0 -Wno-write-strings -fno-exceptions -fmerge-all-constants -std=c++17
LDFLAGS = -L../fetch -lfetch -L../loader -lloader -static-libgcc -static-libstdc++ -lwininet -static -lsodium

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Source files
SRCS = $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
EXECUTABLE = 1806.exe

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INCLUDE_DIR) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

.PHONY: clean
