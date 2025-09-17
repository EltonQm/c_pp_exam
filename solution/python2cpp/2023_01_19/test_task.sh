#!/usr/bin/env bash

# test_task.sh — test interface behavior for task1/task2/task3 C++ executables

# Check for correct usage
if [ $# -lt 2 ]; then
    echo "Usage: $0 task-name ./executable-path"
    echo "Example: $0 task1 ./task1"
    exit 1
fi

TASK_NAME="$1"
EXEC="$2"

# Ensure the executable exists
if [ ! -x "$EXEC" ]; then
    echo "❌ Error: Executable '$EXEC' not found or not executable."
    exit 1
fi

echo "[${TASK_NAME^^}] CLI interface and argument checks for: $EXEC"
echo

# Function to run and show commands with headings
run_check() {
    local desc="$1"
    local cmd="$2"

    echo -e "▶ $cmd"
    eval "$cmd"
    echo -e "\n"
}

# Run tests common to all tasks
run_check "No arguments" "$EXEC"
run_check "--help message" "$EXEC --help"
run_check "Unknown argument" "$EXEC --get-x uniprot.fasta"
run_check "--get-n with dummy file" "$EXEC --get-n dummyFile"
run_check "--get-n with existing file" "$EXEC --get-n uniprot.fasta"

# Optional: Extend behavior based on task name
if [[ "$TASK_NAME" == "task2" ]]; then
    run_check "--get-seq with exact ID" "$EXEC --get-seq 'sp|P59637|VEMP_CVHSA' uniprot.fasta"
    run_check "--get-seq with wildcard ID" "$EXEC --get-seq 'Y14_CVHSA*' uniprot.fasta"
elif [[ "$TASK_NAME" == "task3" ]]; then
    run_check "--gui mode" "$EXEC --gui"
    run_check "--gui with file" "$EXEC --gui uniprot.fasta"
fi
