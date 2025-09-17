CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lz

# Source files
TASK1_SRC = task1.cpp
TASK2_SRC = task2.cpp task2_utils.cpp
TASK3_SRC = task3.cpp task3_utils.cpp task2_utils.cpp
TEST_SRC = test/test_task2_utils.cpp task2_utils.cpp

# Object files
TASK1_OBJ = $(TASK1_SRC:.cpp=.o)
TASK2_OBJ = $(TASK2_SRC:.cpp=.o)
TASK3_OBJ = $(TASK3_SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Executables
TASK1_EXE = task1
TASK2_EXE = task2
TASK3_EXE = task3
TEST_EXE = test_runner

# Default target
all: $(TASK1_EXE) $(TASK2_EXE) $(TASK3_EXE)

# Task 1
$(TASK1_EXE): $(TASK1_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Task 2
$(TASK2_EXE): $(TASK2_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Task 3
$(TASK3_EXE): $(TASK3_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Test
$(TEST_EXE): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Object file rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f *.o test/*.o $(TASK1_EXE) $(TASK2_EXE) $(TASK3_EXE) $(TEST_EXE)

# Test target
test: $(TEST_EXE)
	./$(TEST_EXE)

# Install dependencies (if needed)
install-deps:
	# Add any dependency installation commands here

.PHONY: all clean test install-deps