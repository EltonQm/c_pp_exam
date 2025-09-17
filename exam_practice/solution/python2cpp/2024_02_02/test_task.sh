#!/usr/bin/env bash

# ─── SETTINGS ───────────────────────────────────────────────────────────────
SRC_FILES="task1.cpp task_utils.cpp"
EXE_NAME="task1"
INCLUDE_DIR="argparse/include"

# ─── COLORS ────────────────────────────────────────────────────────────────
YELLOW='\033[1;33m'
RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[0m' # No Color

# ─── COMPILE ────────────────────────────────────────────────────────────────
echo -e "${YELLOW}Compiling: $SRC_FILES → $EXE_NAME${NC}"
g++ -std=c++23 -Wall -Wextra -O2 -I${INCLUDE_DIR} -Wno-unused-parameter $SRC_FILES -o $EXE_NAME -lz

if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed. Please check errors above.${NC}"
    exit 1
fi

# ─── CHECK EXECUTABLE ───────────────────────────────────────────────────────
if [ ! -x "$EXE_NAME" ]; then
    echo -e "${RED}Executable '$EXE_NAME' not found or not executable.${NC}"
    exit 1
fi

# ─── RUN TESTS ──────────────────────────────────────────────────────────────
echo -e "${GREEN}[TASK 1] Testing CLI interface for: ${EXE_NAME}${NC}"

function run_test() {
    echo -e "${YELLOW}▶ $@${NC}"
    eval "$@" 2>&1 | head -n 5
    echo ""
}

# Test 1: No arguments
run_test "./$EXE_NAME"

# Test 2: --help
run_test "./$EXE_NAME --help"

# Test 3: Invalid input file
run_test "./$EXE_NAME --go dummyFile"

# Test 4: Unsupported file extension
run_test "./$EXE_NAME --go test.txt"

# Test 5: Unknown flag
run_test "./$EXE_NAME --dummy uniprot-corona-virus-data-2022-02.dat"

# Test 6: Valid .dat input (file must exist)
run_test "./$EXE_NAME --go uniprot-corona-virus-data-2022-02.dat"

# Test 7: Valid .dat.gz input (file must exist)
run_test "./$EXE_NAME --go corona.dat.gz"

# Test 8: Invalid combination of --go and --doi
run_test "./$EXE_NAME --go file1.dat --doi file2.dat"

echo -e "${GREEN}✔ All CLI tests completed.${NC}"
