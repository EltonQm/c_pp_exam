CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -Iargparse
LDFLAGS = -lz
TESTFLAGS = -I./tests

all: task1 task2 task3 test_task_utils

task1: task1.cpp
	$(CXX) $(CXXFLAGS) task1.cpp -o task1 $(LDFLAGS)

task2: task2.cpp
	$(CXX) $(CXXFLAGS) task2.cpp -o task2 $(LDFLAGS)

task3: task3.cpp
	$(CXX) $(CXXFLAGS) task3.cpp -o task3 $(LDFLAGS)

test_task_utils: tests/test_task_utils.cpp
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) tests/test_task_utils.cpp -o test_task_utils $(LDFLAGS)

test: test_task_utils
	./test_task_utils

clean:
	rm -f task1 task2 task3 test_task_utils *.o *.tab test.obo

.PHONY: all test clean
