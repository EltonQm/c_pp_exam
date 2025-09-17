CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Iargparse/include
LIBS = -lz


# Binaries
T1 = FastaParser1
T2 = FastaParser2
T3 = FastaParser3

# Tests
TEST_DIR = test
TESTS = test_task1 test_task2 test_task3

all: $(T1) $(T2) $(T3)

$(T1): FastaParser1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

$(T2): FastaParser2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

$(T3): FastaParser3.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

# Generic rule for test sources -> test executables
test_%: $(TEST_DIR)/test_%.cpp $(T1) $(T2) $(T3) test/test_helpers.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

.PHONY: test
test: $(TESTS)
	@echo "Running Task1 tests"
	./test_task1
	@echo "Running Task2 tests"
	./test_task2
	@echo "Running Task3 tests"
	./test_task3
	@echo "All tests passed."

clean:
	rm -f $(T1) $(T2) $(T3) $(TESTS)

.PHONY: clean
