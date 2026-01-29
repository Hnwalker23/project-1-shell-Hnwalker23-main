# Makefile for Simple Shell Project
# Operating Systems - Project 1

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Target executable
TARGET = myshell

# Source files
SOURCES = shell.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the shell executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS) *.txt

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
