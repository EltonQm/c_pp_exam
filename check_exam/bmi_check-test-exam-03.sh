#!/usr/bin/env bash
set -o pipefail

########## Color helpers ##########
c_red=$'\033[31m'
c_grn=$'\033[32m'
c_yel=$'\033[33m'
c_rst=$'\033[0m'

msg_error()  { echo -e "${c_red}$*${c_rst}" >&2; }
msg_warn()   { echo -e "${c_yel}$*${c_rst}"; }
msg_yellow() { echo -e "${c_yel}$*${c_rst}"; }
msg_info()   { echo -e "${c_grn}$*${c_rst}"; }

########## File check ##########
# Usage: file_check <filename> [expected_extension]
# Returns 0 if file exists and (if extension given) matches.
file_check() {
    local file="$1"
    local expected_ext="$2"

    if [[ -z "$file" ]]; then
        msg_error "Error: No filename provided!"
        return 1
    fi

    if [[ ! -f "$file" ]]; then
        msg_error "Error: File '$file' does not exist!"
        return 1
    fi

    if [[ -z "$expected_ext" ]]; then
        # No extension expectation supplied: success
        return 0
    fi

    # Normalize extensions (strip leading dot, lower-case)
    local actual="${file##*.}"
    actual=$(tr '[:upper:]' '[:lower:]' <<<"$actual")
    expected_ext="${expected_ext#.}"
    expected_ext=$(tr '[:upper:]' '[:lower:]' <<<"$expected_ext")

    if [[ "$actual" == "$expected_ext" ]]; then
        return 0
    else
        msg_error "Error: '$file' has extension '.$actual' but expected '.$expected_ext'"
        return 1
    fi
}

########## Command executor ##########
# Last argument is the label; preceding args are the command.
mexec() {
    if [[ $# -lt 2 ]]; then
        msg_error "mexec requires at least a command and a label"
        return 1
    fi
    local label="${!#}"
    # Build command array excluding last arg
    local -a cmd=( "${@:1:$#-1}" )

    echo -e "${c_grn}${cmd[*]} (${label})${c_rst}"
    "${cmd[@]}"
}

########## Top-level test harness ##########
tasks() {
    if [[ $# -lt 1 ]]; then
        msg_warn "Usage: bash $0 <cpp-source...>"
        return 1
    fi

    # All arguments except possibly trailing data file path are treated as sources.
    # Convention: we expect a BMI program; just compile everything passed in.
    local -a sources=("$@")

    # Clean old artifacts
    [[ -f task ]]    && rm -f task
    [[ -f out.tab ]] && rm -f out.tab

    msg_yellow "compile"
    mexec g++ -std=c++23 -Wall -Wextra -Wpedantic "${sources[@]}" -o task "compilation"
    if [[ ! -f task ]]; then
        msg_error "Compilation failed!"
        return 1
    fi

    msg_yellow "task1"
    mexec ./task "check usage"
    mexec ./task --help "check help"
    mexec ./task dummy.tab "check wrong file (should error or treat as file depending on your logic)"
    mexec ./task 180 70 "check wrong order"
    mexec ./task 180 "check insufficient arguments"
    mexec ./task 75 180 "check OK"

    msg_yellow "task2"
    mexec ./task 76 180 "Check BMI (76/1.80^2 ≈ 23.5)"

    msg_yellow "task3 - class presence"
    # Search all sources for class (expects 'class Bmi' or similar)
    for src in "${sources[@]}"; do
        grep -n --color=always 'class[[:space:]]\+Bmi' "$src" || true
    done

    msg_yellow "task3 - file processing"
    if [[ -f data.tab ]]; then
        mexec ./task data.tab "check input data from file"
        mexec ./task data.tab out.tab "check output file"
        if [[ -f out.tab ]]; then
            head out.tab
        else
            msg_error "Output file out.tab was not created!"
        fi
    else
        msg_warn "data.tab not found; create it to test file mode (lines: weight<TAB>height)."
    fi
}

tasks "$@"
