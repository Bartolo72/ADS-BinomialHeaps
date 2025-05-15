# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src

# Directories
SRC_DIR = src
OBJ_DIR = build

# Files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Graph/GraphGenerator.cpp \
          $(SRC_DIR)/BinomialHeap/BinomialHeap.cpp \
          $(SRC_DIR)/Dijkstra/Dijkstra.cpp \
          $(SRC_DIR)/Experiments/Experiments.cpp

# Output executable
TARGET = binomial

# Object files (generated from sources)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target to build the executable
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

clean_all:
	rm -rf $(OBJ_DIR) $(TARGET) *.csv

# Rebuild the project
rebuild: clean all

.PHONY: all clean rebuild
