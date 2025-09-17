Gene Ontology Parser
This C++ console application processes Gene Ontology (.obo and .obo.gz) files to analyze obsolete terms, their alternative IDs, subsets, Reactome cross-references, and hierarchical relationships. It is designed for the Programming Expertise course (2025, University of Potsdam) and consists of three incremental versions: task1.cpp, task2.cpp, and task3.cpp.
Features

Task 1 (task1.cpp): Validates command-line arguments for options (--consider-table, --obsoletes-stats, --hierarchical_relationship), input file (.obo or .obo.gz), and optional namespace. Displays a help message for invalid inputs.
Task 2 (task2.cpp): Parses .obo/.obo.gz files to generate a table of obsolete terms with consider or replaced_by IDs (or "NA" if none), with optional namespace filtering. Returns results as a nested vector.
Task 3 (task3.cpp): Implements a C++ class with:
subset: Counts terms per subset (e.g., goslim_generic).
spreadsheet: Filters GO and EC IDs with Reactome xref entries.
obsoletes-stats: Counts obsolete terms with consider, replaced_by, or no alternative IDs.
hierarchical_relationship: Lists GO IDs with is_a and part_of relationships.


File Support: Handles .obo and .obo.gz files using zlib.
Output: Supports terminal output and .tab file output for applicable options.

Directory Structure
.
├── Makefile                # Build tasks and test suite
├── README.md               # This file
├── argparse/               # Optional header-only CLI parser (not used)
├── task1.cpp               # Task 1: Argument parsing and help
├── task2.cpp               # Task 2: Consider/replaced_by table
├── task3.cpp               # Task 3: Class with advanced functionalities
├── tests/
│   ├── catch.hpp           # Catch2 unit testing header
│   └── test_task_utils.cpp # Unit tests for tasks
└── check-exam.sh           # Shell script to test all tasks

Note: task_utils.cpp, task_utils.hpp, and task_utils.o are not used, as the exam does not require shared utilities.
Build Instructions
Requirements

C++17 Compiler: g++ (version 7 or later) or equivalent.
Unix-like Environment: Linux, macOS, or WSL with bash for testing.
zlib: For .obo.gz support (sudo apt install zlib1g-dev on Ubuntu).
Catch2: For unit tests (included as tests/catch.hpp).

Build Commands
# Build all executables and tests
make all

# Build individual tasks
make task1
make task2
make task3

# Build and run tests
make test

# Clean up
make clean

The Makefile compiles task1.cpp, task2.cpp, task3.cpp, and test_task_utils.cpp, linking with zlib for .obo.gz support.
Usage
Task 1 (task1.cpp)
Validates command-line arguments and displays help.
./task1 <option> <filename.obo[.gz]> [namespace]


Options: --consider-table, --obsoletes-stats, --hierarchical_relationship
Example: ./task1 --consider-table go-2020-01.obo molecular_function

Task 2 (task2.cpp)
Generates a table of obsolete terms with consider or replaced_by IDs.
./task2 <option> <filename.obo[.gz]> [namespace]


Options: --consider-table, --replaced-by
Example: ./task2 --consider-table go-2020-01.obo.gz biological_process
Output: Tab-separated, e.g., GO:0000001 GO:0000002 or GO:0000004 NA

Task 3 (task3.cpp)
Supports advanced functionalities with optional file output.
./task3 <option> <filename.obo[.gz]> [namespace] [output.tab]


Options: --consider-table, --replaced-by, --obsoletes-stats, --subset, --spreadsheet, --hierarchical_relationship
Examples:
./task3 --obsoletes-stats go-2020-01.obo stats.tab (outputs: consider 2217, replaced_by 100, no-alternative 2263)
./task3 --subset go-2020-01.obo (outputs: goslim_generic 500, etc.)
./task3 --spreadsheet go-2020-01.obo.gz molecular_function (outputs: GO:0000001 Reactome:12345)



Testing
Unit Tests
Run unit tests using Catch2:
make test

The test_task_utils.cpp file tests argument validation, table parsing, and advanced functionalities using a sample .obo file.
Integration Tests
Use the provided check-exam.sh script to test with a real .obo/.obo.gz file:
bash check-exam.sh task1 task1.cpp go-2020-01.obo
bash check-exam.sh task2 task2.cpp go-2020-01.obo.gz
bash check-exam.sh task3 task3.cpp go-2020-01.obo
bash check-exam.sh all "" go-2020-01.obo

The script validates compilation, argument handling, and output correctness.
Submission
Create a zip archive (surname.zip) containing:

task1.cpp
task2.cpp
task3.cpp
tests/test_task_utils.cpp
Makefile
check-exam.sh

Submit to the exams folder on Moodle or email to the instructor, as specified.
License
No license specified in the exam. For academic use only, as per course requirements.