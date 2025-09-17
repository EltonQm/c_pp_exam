#!/bin/bash

# -------------------- Message Functions -------------------- #

function msg-error() {
    echo -e "\033[31m[ERROR]\033[0m $1"
}
function msg-warn() {
    echo -e "\033[33m[WARNING]\033[0m $1"
}
function msg-info() {
    echo -e "\033[36m[INFO]\033[0m $1"
}
function msg-ok() {
    echo -e "\033[32m[OK]\033[0m $1"
}

# -------------------- Execution Wrapper -------------------- #

function mexec() {
    # Last argument is the label, others are the command
    local all_args=("$@")
    local last_index=$((${#all_args[@]} - 1))
    local msg="${all_args[$last_index]}"
    unset 'all_args[$last_index]'

    echo -e "\n\033[1;34m> ${all_args[*]}  [$msg]\033[0m"
    "${all_args[@]}" 2>&1
}

# -------------------- Test Function -------------------- #

function tasks() {
    if [ -z "$1" ]; then
        msg-warn "Usage: bash $0 task1.cpp | less -r"
        msg-warn "       Requires: sars-cov*.fasta and README.md in current folder"
        return 1
    fi

    local source="$1"

    rm -f task1 out.tab

    msg-info "Compiling with C++23 + argparse"
    mexec g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter task1.cpp task_utils.cpp -o task1 "compiling task1"

    if [ ! -f task1 ]; then
        msg-error "Compilation failed."
        return 1
    fi

    msg-info "=== Running Valid Input Tests ==="
    mexec ./task1 --help "print help"
    mexec ./task1 "no arguments"
    mexec ./task1 --search MAT sars-cov1.fasta "valid search: one file"
    mexec ./task1 --search MAT sars-cov1.fasta sars-cov1B.fasta "valid search: multiple files"
    mexec ./task1 --summary sars-cov1.fasta "valid summary: one file"
    mexec ./task1 --summary sars-cov1.fasta sars-cov2.fasta "valid summary: multiple files"

    msg-info "=== Running Invalid Input Tests ==="
    mexec ./task1 --search "missing pattern and file"
    mexec ./task1 --summary "missing file"
    mexec ./task1 --search MAT "missing file with valid pattern"
    mexec ./task1 --search MAT dummy.fasta "nonexistent file"
    mexec ./task1 --search MAT notes.txt "wrong file extension"
    mexec ./task1 --summary dummy1.csv dummy2.txt "wrong extension and missing"
    mexec ./task1 --foobar "unknown argument"
    mexec ./task1 --search MAT sars-cov1.fasta README.md "README.md is not FASTA"

    msg-info "=== Conflicting Options Test ==="
    mexec ./task1 --search MAT sars-cov1.fasta --summary sars-cov2.fasta "conflicting --search + --summary"

    msg-info "=== Dummy Entry Test ==="
    mexec ./task1 --get-entry dummy sars-cov1.fasta "dummy --get-entry (if implemented)"

    msg-info "=== All Tests Completed ==="
}

# -------------------- Execute -------------------- #

tasks "$@"
