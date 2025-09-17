#!/bin/bash

# Helper functions from check-test-exam-02.sh
function msg-error {
    echo -e "\033[31m$1\033[0m"
}    
    
function msg-warn  {
    echo -e "\033[33m$1\033[0m"
}
function msg-yellow  {
    echo -e "\033[33m$1\033[0m"
}

function mexec {
    ## last argument is the message
    msg=${!#}
    ## remove it from compilation args
    local args=("$@")
    set -- "${args[@]:0:$((${#args[@]}-1))}"
    echo -e "\033[32m$@ ($msg)\033[0m"
    "$@"
}

function file_check () {
    # Check if an argument was provided
    if [ -z "$1" ]; then
        msg-error "Error: No filename provided!"
        return 1
    fi
    if [ -f "$1" ]; then
        filename=$(basename -- "$1")
        extension="${filename##*.}"
        extension=$(echo "$extension" | awk '{print tolower($0)}') ;
        givenext=$(echo "$2" | awk '{print tolower($0)}') ;
        if [ -z "$2" ]; then
            # no extension given, file exists
            return 0
        elif [ ".$extension" == "$givenext" ]; then # Added a dot for consistent comparison with givenext e.g. ".obo"
            return 0
        else
            msg-error "Error: $1 has not the required extension $2 ('$extension' != '$givenext')"
            return 1
        fi
    else
        msg-error "Error: File '$1' not found."
        return 1
    fi
}

# --- Task 1 Check ---
function task1_check {
    local cpp_file="$1"
    local obo_file="$2"

    msg-yellow "--- Checking Task 1: Layout application, command line arguments ---"

    if ! file_check "$cpp_file" ".cpp"; then return 1; fi
    if ! file_check "$obo_file" ".obo"; then return 1; fi

   # mexec g++ -std=c++23 -Iargparse/include "$cpp_file" -o task1_app -lz "Compilation of Task 1"
    mexec g++ -std=c++23 -Iargparse/include task1.cpp task_utils.cpp -o task1 -lz "Compile task1"
    if ! [ -f "task1_app" ]; then
        msg-error "Compilation failed for Task 1!"
        return 1
    fi

    mexec ./task1_app "Run without arguments (should show help)" | head -n 5
    mexec ./task1_app --help "Run with --help" | head -n 5
    mexec ./task1_app --invalid-command "$obo_file" "Run with invalid command name" | head -n 5
    mexec ./task1_app --consider-table dummy.obo "Run with invalid OBO filename" | head -n 5
    mexec ./task1_app --consider-table "$obo_file" invalid_namespace "Run with invalid namespace" | head -n 5
    mexec ./task1_app --consider-table "$obo_file" "Run with valid --consider-table (no namespace)" | head -n 5
    mexec ./task1_app --consider-table "$obo_file" molecular_function "Run with valid --consider-table (with namespace)" | head -n 5
}

# --- Task 2 Check ---
function task2_check {
    local cpp_file="$1"
    local obo_file="$2"

    msg-yellow "--- Checking Task 2: Opening Obofile and creating a consider table ---"

    if ! file_check "$cpp_file" ".cpp"; then return 1; fi
    if ! file_check "$obo_file" ".obo"; then return 1; fi

    mexec g++ -std=c++23 -Iargparse/include "$cpp_file" -o task2_app -lz "Compilation of Task 2"
    if ! [ -f "task2_app" ]; then 
        msg-error "Compilation failed for Task 2!"
        return 1
    fi
    
    mexec ./task2_app --consider-table "$obo_file" "Generate consider table (full)" | head -n 20
    mexec ./task2_app --consider-table "$obo_file" molecular_function "Generate consider table (molecular_function namespace)" | head -n 20
    mexec ./task2_app --consider-table "$obo_file" | grep "NA" "Check for NA entries in consider table" | head -n 5
}

# --- Task 3 Check ---
function task3_check {
    local cpp_file="$1"
    local obo_file="$2"

    msg-yellow "--- Checking Task 3: Advanced terminal application ---"

    if ! file_check "$cpp_file" ".cpp"; then return 1; fi
    if ! file_check "$obo_file" ".obo"; then return 1; fi

    mexec grep -A 10 -E '^\s*class' "$cpp_file" "Check for C++ class definition (optional extra point)"

    mexec g++ -std=c++23 -Iargparse/include "$cpp_file" -o task3_app -lz "Compilation of Task 3"
    if ! [ -f "task3_app" ]; then 
        msg-error "Compilation failed for Task 3!"
        return 1
    fi

    msg-yellow "--- Testing --obsolete-stats ---"
    mexec ./task3_app --obsolete-stats "$obo_file" "Get obsolete stats" | head -n 10
    mexec ./task3_app --obsolete-stats "$obo_file" | grep "consider" "Check 'consider' count"
    mexec ./task3_app --obsolete-stats "$obo_file" | grep "no-consider" "Check 'no-consider' count"

    msg-yellow "--- Testing optional outfile argument (.tab) ---"
    local output_file="consider-ids.tab"
    if [ -f "$output_file" ]; then rm "$output_file"; fi # Clean up previous run's output file

    mexec ./task3_app --consider-table "$obo_file" "$output_file" "Write consider table to $output_file"
    if [ -f "$output_file" ]; then
        msg-yellow "Content of $output_file:"
        head -n 10 "$output_file"
        msg-yellow "Checking if $output_file contains 'GO:0000044' and 'NA' as per example (if present in your obo file):"
        grep "GO:0000044" "$output_file" || true
        grep "NA" "$output_file" || true
    else
        msg-error "Output file $output_file was not created!"
    fi
}

# Main execution logic
if [ -z "$3" ]; then
    msg-warn "Usage: bash $0 <task-name> <cpp-file> <obo-file>"
    msg-warn "Available tasks: task1_check, task2_check, task3_check"
    exit 1
fi

task_to_run="$1"
cpp_file="$2"
obo_file="$3"

if declare -f "$task_to_run" > /dev/null; then
    "$task_to_run" "$cpp_file" "$obo_file"
else
    msg-error "Unknown task: $task_to_run"
    msg-warn "Available tasks: task1_check, task2_check, task3_check"
    exit 1
fi
