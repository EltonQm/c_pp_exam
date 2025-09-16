CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2 -Iargparse

all: task1 task2 task3

task1: task1.cpp
	$(CXX) $(CXXFLAGS) task1.cpp -o task1

task2: task2.cpp task2_utils.o
	$(CXX) $(CXXFLAGS) task2.cpp task2_utils.o -o task2

task3: task3.cpp task3_utils.o
	$(CXX) $(CXXFLAGS) task3.cpp task3_utils.o -o task3

task2_utils.o: task2_utils.cpp task2_utils.hpp
	$(CXX) $(CXXFLAGS) -c task2_utils.cpp

task3_utils.o: task3_utils.cpp task3_utils.hpp
	$(CXX) $(CXXFLAGS) -c task3_utils.cpp

clean:
	rm -f *.o task1 task2 task3
