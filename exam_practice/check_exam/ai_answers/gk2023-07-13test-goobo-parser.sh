#!/bin/bash

# === Colorful Output Helpers ===
function msg-error() {
    echo -e "\033[31m$1\033[0m" >&2
}

function msg-warn() {
    echo -e "\033[33m$1\033[0m"
}

function msg-yellow() {
    echo -e "\033[33m$1\033[0m"
}

# === Safe Command Execution with Last Arg as Label ===
function mexec() {
    local args=("$@")
    local last_index=$((${#args[@]} - 1))
    local msg="${args[$last_index]}"
    unset 'args[$last_index]'
    echo -e "\033[32m${args[*]} ($msg)\033[0m"
    "${args[@]}" 2>&1
}

# === Check File Exists and Has .obo Extension ===
function file_check() {
    local file=$1
    if [ -z "$file" ]; then
        msg-error "Error: No filename provided!"
        return 1
    fi
    if [ ! -f "$file" ]; then
        msg-error "Error: File '$file' does not exist!"
        return 1
    fi
    local ext="${file##*.}"
    ext="${ext,,}"  # lowercase
    if [ "$ext" != "obo" ]; then
        msg-error "Error: '$file' does not have the required .obo extension"
        return 1
    fi
    return 0
}

# === Task 1 Tests ===
function test_task1() {
    local cpp_file=$1
    local obo_file=$2
    msg-yellow "🧪 Testing Task 1: $cpp_file"

    rm -f task1
   #mexec g++ -std=c++23 -Iargparse/include "$cpp_file" -o task1 -lz "Compile task1"
    mexec g++ -std=c++23 -Iargparse/include task1.cpp task_utils.cpp -o task1 -lz "Compile task1"
    if [ ! -f task1 ]; then
        msg-error "❌ Compilation failed for $cpp_file"
        return 1
    fi

    msg-yellow "📋 Task 1: Argument and file validation"
    mexec ./task1 "no args → usage"
    mexec ./task1 --help "invalid arg → usage/help"
    mexec ./task1 dummy.obo "nonexistent file"
    mexec ./task1 "$obo_file" "valid obo file run"
}

# === Task 2 Tests ===
function test_task2() {
    local cpp_file=$1
    local obo_file=$2
    msg-yellow "🧪 Testing Task 2: $cpp_file"

    rm -f task2
    mexec g++ -std=c++23 -Iargparse/include "$cpp_file" -o task2 -lz "Compile task2"

    if [ ! -f task2 ]; then
        msg-error "❌ Compilation failed for $cpp_file"
        return 1
    fi

    if file_check "$obo_file"; then
        mexec ./task2 "$obo_file" "run task2 with valid file"
    else
        msg-error "❌ Invalid .obo file for task2"
    fi
}

# === Task 3 Tests ===
function test_task3() {
    local cpp_file=$1
    local obo_file=$2
    msg-yellow "🧪 Testing Task 3: $cpp_file"

    rm -f task3
    mexec g++ -std=c++23 -Iargparse/include "$cpp_file" -o task3 -lz "Compile task3"
    if [ ! -f task3 ]; then
        msg-error "❌ Compilation failed for $cpp_file"
        return 1
    fi

    if file_check "$obo_file"; then
        mexec ./task3 "$obo_file" META1 META2 "run task3 with MetaCyc filters"
    else
        msg-error "❌ Invalid .obo file for task3"
    fi
}

# === Dispatcher ===
function tasks() {
    if [ $# -lt 3 ]; then
        msg-warn "Usage: bash $0 task-name cpp-file obo-file"
        echo "Where task-name is: task1, task2, task3, or all"
        return 1
    fi

    local task_name=$1
    local cpp_file=$2
    local obo_file=$3

    if [ ! -f "$cpp_file" ]; then
        msg-error "Error: C++ file '$cpp_file' does not exist!"
        return 1
    fi

    case "$task_name" in
        task1)
            test_task1 "$cpp_file" "$obo_file"
            ;;
        task2)
            test_task2 "$cpp_file" "$obo_file"
            ;;
        task3)
            test_task3 "$cpp_file" "$obo_file"
            ;;
        all)
            test_task1 "$cpp_file" "$obo_file"
            test_task2 "$cpp_file" "$obo_file"
            test_task3 "$cpp_file" "$obo_file"
            ;;
        *)
            msg-error "Invalid task: '$task_name'. Use task1, task2, task3, or all."
            ;;
    esac
}

# === Run Script ===
tasks "$@"
