# Compiler settings - Can be gcc, cl.exe, etc.
CXX = g++

# Compiler flags:
# -Wall : This enables all the warnings about constructions that some users consider questionable
# -Wextra : This enables some extra warning flags that are not enabled by -Wall
# -std=c++17 : We use C++17 standard
# JSON_HAS_CPP_17=0 avoids trying to include <optional> and <variant> which fail on GCC 6.3 MinGW
CXXFLAGS = -Wall -Wextra -std=c++17 -DJSON_HAS_CPP_17=0 -DJSON_HAS_EXPERIMENTAL_CPP_14=0

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source and Object Files
# Finds all .cpp files in the SRC_DIR
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Replaces .cpp with .o and prepends the BUILD_DIR path
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Target executable name
TARGET = $(BUILD_DIR)/DeskLock.exe

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean up build files (Cross-platform compatible for MinGW sh)
clean:
	rm -rf $(BUILD_DIR)
