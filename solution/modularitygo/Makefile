# ---- Compiler & flags --------------------------------------------------------
CXX      ?= g++
CXXSTD    = -std=c++20
WARN      = -Wall -Wextra -Wno-unused-parameter
OPT       = -O2
INCLUDES  = -Iargparse/include
LIBS      = -lz

CXXFLAGS := $(CXXSTD) $(WARN) $(OPT) $(INCLUDES) -MMD -MP

# ---- Targets ----------------------------------------------------------------
.PHONY: all clean

all: task1 task2 task3

# task1 depends on task_utils
task1: task1.o task_utils.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# task2 depends on task2_utils and task_utils
task2: task2.o task2_utils.o task_utils.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# task3 depends on task3_utils, task2_utils, and task_utils (as per your original one-liner)
task3: task3.o task3_utils.o task2_utils.o task_utils.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# ---- Pattern rules -----------------------------------------------------------
# Compile any .cpp to .o with automatic dep generation (.d files)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---- Headers / Deps ----------------------------------------------------------
# Include all generated dependency files if they exist
-include $(wildcard *.d)

# ---- Clean -------------------------------------------------------------------
clean:
	rm -f task1 task2 task3 *.o *.d
