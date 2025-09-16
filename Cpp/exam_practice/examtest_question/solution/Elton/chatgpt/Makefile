# =========================
#  Compiler / Flags
# =========================
CXX       := g++
STD       := -std=c++23
WARN      := -Wall -Wextra -Wpedantic
OPT       := -O2
# Enable debug build with: make DEBUG=1
ifeq ($(DEBUG),1)
    OPT := -O0 -g
endif

# Path to argparse headers (adjust if different)
ARGPARSE_INC := -Iargparse/include

CXXFLAGS := $(STD) $(WARN) $(OPT) $(ARGPARSE_INC)
LDFLAGS  :=
LIBS     := -lz

# =========================
#  Sources / Objects
# =========================
TASK1_SRCS := task1.cpp task2_utils.cpp
TASK2_SRCS := task2.cpp task2_utils.cpp
TASK3_SRCS := task3.cpp task2_utils.cpp task3_utils.cpp

TASK1_OBJS := $(TASK1_SRCS:.cpp=.o)
TASK2_OBJS := $(TASK2_SRCS:.cpp=.o)
TASK3_OBJS := $(TASK3_SRCS:.cpp=.o)

# Tests (adjust/add more test files as needed)
TEST_SRCS  := tests/test_task2_utils.cpp task2_utils.cpp
TEST_OBJS  := $(TEST_SRCS:.cpp=.o)
TEST_BIN   := test_task2_utils

# Default target
.PHONY: all
all: task1 task2 task3

# =========================
#  Build Rules
# =========================
task1: $(TASK1_OBJS)
	$(CXX) $(CXXFLAGS) $(TASK1_OBJS) -o $@ $(LIBS)

task2: $(TASK2_OBJS)
	$(CXX) $(CXXFLAGS) $(TASK2_OBJS) -o $@ $(LIBS)

task3: $(TASK3_OBJS)
	$(CXX) $(CXXFLAGS) $(TASK3_OBJS) -o $@ $(LIBS)

$(TEST_BIN): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $@ $(LIBS)

# Pattern rule for compiling any .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Allow test sources to find local headers
tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -Itests -c $< -o $@

# =========================
#  Convenience Targets
# =========================
.PHONY: test
test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: clean
clean:
	rm -f task1 task2 task3 $(TEST_BIN) $(TASK1_OBJS) $(TASK2_OBJS) $(TASK3_OBJS) $(TEST_OBJS)

.PHONY: rebuild
rebuild: clean all

# Optional: print variables for debugging
.PHONY: vars
vars:
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "TASK1_OBJS = $(TASK1_OBJS)"
	@echo "TASK2_OBJS = $(TASK2_OBJS)"
	@echo "TASK3_OBJS = $(TASK3_OBJS)"
