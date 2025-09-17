# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2
INCLUDES := -Iargparse/include
LIBS := -lz

# Executables to build
TARGETS := task1 task2 task3

# Default target: build all
all: $(TARGETS)

# Build task1.cpp → task1
task1: task1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

# Build task2.cpp → task2
task2: task2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

# Build task3.cpp → task3
task3: task3.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

# Clean all build artifacts
clean:
	rm -f $(TARGETS)
