#!/bin/bash

# --- Helper functions ---
function file_check () {
    if [ -z "$1" ]; then
        msg_error "Error: No filename provided!"
        return 1
    fi
    if [ -f "$1" ]; then
        if [ -z "$2" ]; then
            return 0
        else
            filename=$(basename -- "$1")
            extension=".$(echo "${filename##*.}" | awk '{print tolower($0)}')"
            givenext=".$(echo "$2" | awk '{print tolower($0)}')"
            if [ "$extension" == "$givenext" ]; then
                return 0
            else
                msg_error "Error: $1 does not have the required extension $2 ('$extension' != '$givenext')"
                return 1
            fi
        fi
    fi
    msg_error "Error: File does not exist!"
    return 1
}

function msg_error()   { echo -e "\033[31m❌ $1\033[0m"; }
function msg_success() { echo -e "\033[32m✅ $1\033[0m"; }
function msg_warn()    { echo -e "\033[33m⚠️  $1\033[0m"; }
function msg_info()    { echo -e "\033[36mℹ️  $1\033[0m"; }

# --- Track results ---
declare -a TEST_RESULTS

function mexec {
    # Store the last argument as message
    local last_index=$(($# - 1))
    local msg="${!#}" # equivalent to ${@:$#}
    
    # Extract the command arguments (everything except the last)
    local cmd=("${@:1:$last_index}")

    echo -e "\033[34m➡️  ${cmd[*]} ($msg)\033[0m"
    output=$("${cmd[@]}" 2>&1)
    local status=$?
    if [ $status -eq 0 ]; then
        msg_success "$msg"
    else
        msg_error "$msg"
    fi
    echo "$output"
    echo "----"
    TEST_RESULTS+=("$msg:$status")
}


function tasks {
    if [ -z "$3" ]; then
        msg_warn "Usage: bash $0 cpp-file fasta-file1 fasta-file2 [optional.cpp]"
        return 0
    else
        if [ -f task ]; then rm task; fi
        msg_info "Compiling..."

        mexec g++ -std=c++17 -Iargparse/include $4 $1 -o task "compilation"

        if ! [ -f "task" ]; then
            msg_error "Compilation failed!"
            return 1
        fi

        msg_info "Running tests..."

        # Task 1: Basic usage
        mexec ./task "check usage"
        mexec ./task --help "check help"
        mexec ./task --search MA dummy.fasta "check wrong file"
        mexec ./task --dummy MA $2 "check wrong argument --dummy"
        mexec ./task --search $2 "check --search missing pattern"
        mexec ./task --summary MA $2 "check --summary wrong argument"

        # Task 2: Search tests
        mexec ./task --search MA $2 "check --search OK - task 2"
        mexec ./task --search MSD $2 "check --search OK - task 2"

        # Task 3: Multi-file support
        mexec grep --color class $1 "check for class - task 3"
        mexec ./task --search MSD $2 $3 "check --search OK multi-file - task 3"
        mexec ./task --summary $2 "check --summary OK - task 3"
        mexec ./task --summary $2 | grep -E '76|VEMP_CVHSA' "check summary contains expected line"

        msg_info "Test summary:"
        for result in "${TEST_RESULTS[@]}"; do
            label="${result%%:*}"
            code="${result##*:}"
            if [ "$code" -eq 0 ]; then
                echo -e "✅ $label"
            else
                echo -e "❌ $label"
            fi
        done
    fi
}

# Run the test pipeline
tasks "$@"
