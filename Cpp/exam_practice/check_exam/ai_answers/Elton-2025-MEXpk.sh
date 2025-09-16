#!/usr/bin/env bash
set -o pipefail

############################
# Color / message helpers  #
############################
c_red=$'\033[31m'
c_grn=$'\033[32m'
c_yel=$'\033[33m'
c_rst=$'\033[0m'

msg-error () { echo -e "${c_red}$*${c_rst}" >&2; }
msg-warn  () { echo -e "${c_yel}$*${c_rst}"; }
msg-info  () { echo -e "${c_grn}$*${c_rst}"; }
msg-head  () { echo -e "${c_yel}==== $* ====${c_rst}"; }

########################################
# Execute a command with an annotation #
########################################
mexec () {
    local label=$1; shift
    echo -e "${c_grn}$*${c_rst}   (${label})"
    "$@"
}

#############################################
# Check that a file exists & has right ext. #
#############################################
file_check () {
    local f=$1
    if [[ -z $f ]]; then
        msg-error "Error: No filename provided!"
        return 1
    fi
    if [[ ! -f $f ]]; then
        msg-error "Error: File '$f' does not exist!"
        return 1
    fi
    local base ext
    base=$(basename -- "$f")
    ext=${base##*.}
    ext=$(tr '[:upper:]' '[:lower:]' <<<"$ext")
    if [[ $ext == "obo" || $ext == "gz" ]]; then
        return 0
    fi
    msg-error "Error: $f does not have .obo or .obo.gz extension"
    return 1
}

################################################
# Task 1 tests (argument parsing / basic help) #
################################################
test_task1 () {
    local cpp_file=$1
    local obo_file=$2
    msg-head "Task 1: $cpp_file"

    rm -f task1
    mexec "compile task1" g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 -Iargparse/include "$cpp_file" -o task1 -lz || {
        msg-error "Compilation failed (task1)"
        return 1
    }

    msg-info "Task 1: Argument validation"
    mexec "no args"        ./task1 || true
    mexec "--help"         ./task1 --help | head -n 8
    mexec "invalid option" ./task1 --not-an-option "$obo_file" || true
    mexec "missing file"   ./task1 --consider-table missing.obo || true
    if file_check "$obo_file"; then
        mexec "consider-table all namespaces" ./task1 --consider-table "$obo_file" | head -n 5
        mexec "consider-table valid ns"       ./task1 --consider-table "$obo_file" molecular_function | head -n 5
    fi
}

############################################################
# Task 2 tests (consider / replaced_by tables from parser) #
############################################################
test_task2 () {
    local cpp_file=$1
    local obo_file=$2
    msg-head "Task 2: $cpp_file"

    rm -f task2
    mexec "compile task2" g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 -Iargparse/include "$cpp_file" -o task2 -lz || {
        msg-error "Compilation failed (task2)"
        return 1
    }

    if file_check "$obo_file"; then
        msg-info "Task 2: Table checks"
        mexec "consider all"     ./task2 --consider-table "$obo_file" | head -n 10
        mexec "consider MF"      ./task2 --consider-table "$obo_file" molecular_function | head -n 10
        mexec "consider CC"      ./task2 --consider-table "$obo_file" cellular_component | head -n 10
        mexec "consider BP"      ./task2 --consider-table "$obo_file" biological_process | head -n 10
        mexec "replaced-by all"  ./task2 --replaced-by "$obo_file" | head -n 10
    else
        msg-error "Skipping Task 2: invalid OBO file."
    fi
}

#######################################################################
# Task 3 tests (class-based advanced: subsets, Reactome, stats, rels) #
#######################################################################
test_task3 () {
    local cpp_main=$1      # e.g. task3.cpp
    local obo_file=$2
    shift 2
    local extra_sources=("$@") # e.g. task3_utils.cpp task2_utils.cpp

    msg-head "Task 3: Advanced (sources: $cpp_main ${extra_sources[*]})"

    rm -f task3
    mexec "compile task3" g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 \
        -Iargparse/include \
        "$cpp_main" "${extra_sources[@]}" -o task3 -lz || {
            msg-error "Compilation failed (task3)"
            return 1
        }

    # Quick class presence sanity
    mexec "grep class" grep -n 'class[[:space:]]\+GOStats' "$cpp_main" || true

    if file_check "$obo_file"; then
        msg-info "Task 3: --obsolete-stats"
        mexec "obsolete stats"      ./task3 --obsolete-stats "$obo_file" | head -n 10

        msg-info "Task 3: --subset-counts"
        mexec "subset counts"       ./task3 --subset-counts "$obo_file" | head -n 10

        msg-info "Task 3: --xrefsearch"
        mexec "xrefsearch"          ./task3 --xrefsearch "$obo_file" | head -n 10

        msg-info "Task 3: --hierarchy (limited)"
        mexec "hierarchy limit=12"  ./task3 --hierarchy --limit 12 "$obo_file"

    else
        msg-error "Skipping Task 3: invalid OBO file."
        return 1
    fi
}

############################################
# Orchestrator for selecting which tests  #
############################################
run_tasks () {
    if [[ $# -lt 3 ]]; then
        msg-warn "Usage: bash $0 {task1|task2|task3|all} <cpp-file(s)> <obo-file>"
        msg-warn "Examples:"
        msg-warn "  bash $0 task1 GooboParser1.cpp ontology.obo"
        msg-warn "  bash $0 task3 task3.cpp ontology.obo task3_utils.cpp task2_utils.cpp"
        return 1
    fi

    local task_name=$1
    shift
    # Last argument is the OBO file; everything before are C++ source(s)
    local obo_file=${!#}
    local cpp_sources=("$@")
    unset 'cpp_sources[${#cpp_sources[@]}-1]'

    case "$task_name" in
        task1)
            if [[ ${#cpp_sources[@]} -ne 1 ]]; then
                msg-error "Task1 expects exactly 1 C++ file."
                return 1
            fi
            test_task1 "${cpp_sources[0]}" "$obo_file"
            ;;
        task2)
            if [[ ${#cpp_sources[@]} -ne 1 ]]; then
                msg-error "Task2 expects exactly 1 C++ file."
                return 1
            fi
            test_task2 "${cpp_sources[0]}" "$obo_file"
            ;;
        task3)
            if [[ ${#cpp_sources[@]} -lt 1 ]]; then
                msg-error "Task3 expects at least 1 C++ main file plus optional extra sources."
                return 1
            fi
            local main_cpp=${cpp_sources[0]}
            local rest=("${cpp_sources[@]:1}")
            test_task3 "$main_cpp" "$obo_file" "${rest[@]}"
            ;;
        all)
            # Hard-coded conventional filenames (adjust if different)
            test_task1 "GooboParser1.cpp" "$obo_file"
            test_task2 "GooboParser2.cpp" "$obo_file"
            test_task3 "task3.cpp"        "$obo_file" "task3_utils.cpp" "task2_utils.cpp"
            ;;
        *)
            msg-error "Invalid task '$task_name'. Use task1 | task2 | task3 | all."
            return 1
            ;;
    esac
}

run_tasks "$@"
