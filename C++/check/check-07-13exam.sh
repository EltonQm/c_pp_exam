#!/usr/bin/env bash

# Usage: bash check-exam.sh task1 GooboParser1.cpp
# This version checks whether the student solution can handle both .obo and .obo.gz files directly.

function mexec {
    msg="$@"
    echo -e "\033[32m$msg\033[0m"
    $@
}

function message {
    echo -e "\033[33m$1 \033[0m"
}

function compile_cpp {
    cpp_file=$1
    exe_file="${cpp_file%.cpp}.out"
    g++ -std=c++17 "$cpp_file" -o "$exe_file"
    echo "$exe_file"
}

function task1 {
    exe=$(compile_cpp "$1")
    message "TASK 1: Help and argument checking"

    message "Run without arguments"
    mexec ./"$exe"

    message "Run with invalid option"
    mexec ./"$exe" --invalid-option test.obo

    message "Run with missing file"
    mexec ./"$exe" --consider-table nonexistent.obo

    message "Run with valid file and wrong namespace"
    mexec ./"$exe" --consider-table test.obo not_a_namespace
}

function task2 {
    exe=$(compile_cpp "$1")
    message "TASK 2: --consider-table"

    message "Run with plain .obo file (no namespace)"
    mexec ./"$exe" --consider-table test.obo

    message "Run with plain .obo file and namespace"
    mexec ./"$exe" --consider-table test.obo molecular_function

    message "Run with .gz file (no namespace)"
    mexec ./"$exe" --consider-table test.obo.gz

    message "Run with .gz file and namespace"
    mexec ./"$exe" --consider-table test.obo.gz biological_process
}

function task3 {
    exe=$(compile_cpp "$1")
    message "TASK 3: --obsolete-stats and output file"

    message "Run --obsolete-stats with plain .obo"
    mexec ./"$exe" --obsolete-stats test.obo

    message "Run --obsolete-stats with .gz"
    mexec ./"$exe" --obsolete-stats test.obo.gz

    message "Run --consider-table with output file"
    mexec ./"$exe" --consider-table test.obo molecular_function consider-output.tab

    if [[ -f consider-output.tab ]]; then
        echo "Output file content:"
        head -n 10 consider-output.tab
        rm consider-output.tab
    fi
}

if [ -z "$2" ]; then
    echo "Usage: task1|task2|task3 FILENAME.cpp"
else
    "$@" | less -r
fi
