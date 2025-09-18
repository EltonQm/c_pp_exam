# Makefile — builds task1, task2, task3 (C++23 + argparse)
CXX       ?= g++
CXXFLAGS  ?= -std=c++23 -O2 -Wall -Wextra -Wpedantic -Iexternal/argparse/include -Iinclude
LDFLAGS   ?=
BLD       := build

APPS := task1 task2 task3

LIBOBJ := \
  $(BLD)/task_utils.o \
  $(BLD)/task2_utils.o \
  $(BLD)/task3_utils.o

all: $(APPS)

$(BLD):
	mkdir -p $(BLD)

# ---- Object rules ----
$(BLD)/%.o: src/%.cpp | $(BLD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---- Executables ----
task1: $(BLD)/task1.o $(BLD)/task_utils.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

task2: $(BLD)/task2.o $(BLD)/task_utils.o $(BLD)/task2_utils.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

task3: $(BLD)/task3.o $(BLD)/task_utils.o $(BLD)/task3_utils.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# ---- Phony ----
clean:
	rm -rf $(BLD) $(APPS)

.PHONY: all clean
