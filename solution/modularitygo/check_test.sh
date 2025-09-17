#!/bin/bash

# ───────────────────────────────────────────────────────────────
# Function: file_check
# Usage   : file_check <filename> [expected_extension]
# Purpose : Validate that the file exists and optionally matches a file extension
# ───────────────────────────────────────────────────────────────
function file_check() {
    if [ -z "$1" ]; then
        msg-error "Error: No filename provided!"
        return 1
    fi

    if [ -f "$1" ]; then
        filename=$(basename -- "$1")
        extension="${filename##*.}"
        extension=".$(echo "$extension" | awk '{print tolower($0)}')"
        givenext=".$(echo "$2" | awk '{print tolower($0)}')"

        if [ -z "$2" ]; then
            return 0
        elif [ "$extension" == "$givenext" ]; then
            return 0
        else
            msg-error "Error: '$1' does not have the required extension '$2' (found '$extension')"
            return 1
        fi
    fi

    msg-error "Error: File '$1' not found or not a regular file!"
    return 1
}

# ─────────────────────────────
# mexec - Colored command runner with message
# ─────────────────────────────
function mexec() {
    msg=${!#}
    local args=("$@")
    set -- "${args[@]:0:$((${#args[@]} - 1))}"
    echo -e "\033[32m$@ ($msg)\033[0m"
    "$@" 2>&1
}

# ─────────────────────────────
# Message printing utilities
# ─────────────────────────────
function msg-error() {
    echo -e "\033[31m$1\033[0m"
}
function msg-warn() {
    echo -e "\033[33m$1\033[0m"
}
function msg-yellow() {
    echo -e "\033[33m$1\033[0m"
}

# ─────────────────────────────────────────────────────────────────
# tasks - Compile and test task1/task2/task3 binaries
# Usage: ./this_script.sh task1|task2|task3 your_cpp_file [obo_file]
# ─────────────────────────────────────────────────────────────────
function tasks() {
    if [ -z "$1" ]; then
        msg-warn "Usage: bash $0 task1|task2|task3 cpp-file [obo-file]"
        return 0
    fi

    TASK="$1"
    CPP_FILE="$2"
    OBO_FILE="$3"

    [ -f task ] && rm task
    [ -f out.tab ] && rm out.tab

    msg-yellow "Compiling $TASK..."

    if [[ "$TASK" == "task1" ]]; then
        g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter "$CPP_FILE" task_utils.o -o task1 -lz 2>&1 | head -n 5
        [ ! -f task1 ] && msg-error "Compilation failed!" && return 1

        msg-yellow "Running task1 tests"
        mexec ./task1 "Show usage"
        mexec ./task1 --help "Show help"
        mexec ./task1 --dummy "$OBO_FILE" "Unknown flag test"
        mexec ./task1 --consider-table biological_process "$OBO_FILE" "Valid --consider-table"
        mexec ./task1 --obsolete-stats cellular_component dummy.obo "Valid --obsolete-stats with dummy.obo"
        mexec ./task1 --obsolete-stats cellular_component "$OBO_FILE" "Valid --obsolete-stats with actual obo file"
        mexec ./task1 --dummy -f "$OBO_FILE" "Dummy flag with -f"
        mexec ./task1 --consider-table biological_process -f "$OBO_FILE" "Consider-table with -f"
        mexec ./task1 --obsolete-stats cellular_component -f dummy.obo "Obsolete-stats with dummy.obo and -f"
        mexec ./task1 --obsolete-stats cellular_component -f "$OBO_FILE" "Obsolete-stats with real obo and -f"
        

    elif [[ "$TASK" == "task2" ]]; then
        g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter "$CPP_FILE" task2_utils.o -o task2 -lz 2>&1 | head -n 5
        [ ! -f task2 ] && msg-error "Compilation failed!" && return 1

        msg-yellow "Running task2 tests"
        mexec ./task2 "Show usage"
        mexec ./task2 --consider-table molecular_function "$OBO_FILE" "Valid --consider-table"

    elif [[ "$TASK" == "task3" ]]; then
        g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter "$CPP_FILE" task3_utils.o -o task3 -lz 2>&1 | head -n 5
        [ ! -f task3 ] && msg-error "Compilation failed!" && return 1

        msg-yellow "Running task3 tests"
        mexec ./task3 --tab-metacyc "$OBO_FILE" "Tab-metacyc full test"
        mexec ./task3 --tab-metacyc "$OBO_FILE" | grep GO:0000016 "Check for specific GO term"

    else
        msg-error "Unknown task: $TASK (expected: task1, task2, or task3)"
        return 1
    fi
}

# Execute with passed arguments
tasks "$@"
