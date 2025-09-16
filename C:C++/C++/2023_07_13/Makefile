CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2 -Iargparse/include
LIBS = -lz

all: task1 task2 task3

task1: task1.cpp task1_utils.o
	$(CXX) $(CXXFLAGS) task1.cpp task1_utils.o -o task1 $(LIBS)

task2: task2.cpp task2_utils.o
	$(CXX) $(CXXFLAGS) task2.cpp task2_utils.o -o task2 $(LIBS)

task3: task3.cpp task3_utils.o
	$(CXX) $(CXXFLAGS) task3.cpp task3_utils.o -o task3 $(LIBS)

task_utils.o: task_utils.cpp task_utils.hpp
	$(CXX) $(CXXFLAGS) -c task_utils.cpp -o task_utils.o

# Add to the top if not already present
#TEST_DIR = tests

# Test target
#test: test_task_utils
#	./test_task_utils

#test_task_utils: $(TEST_DIR)/test_task_utils.cpp task_utils.o
#	$(CXX) $(CXXFLAGS) -I$(TEST_DIR) $(TEST_DIR)/test_task_utils.cpp task_utils.o -o test_task_utils $(LIBS)


clean:
	rm -f task1 task2 task3 *.o
