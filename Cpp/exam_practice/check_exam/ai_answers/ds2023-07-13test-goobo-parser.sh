#!/bin/bash

# Test script for MS-PEX-2023-Test-Klausur-2023-07-13_obo exam implementation
# Checks all three tasks described in the exam PDF

function msg-error {
    echo -e "\033[31m$1\033[0m"
}    
    
function msg-warn {
    echo -e "\033[33m$1\033[0m"
}

function msg-success {
    echo -e "\033[32m$1\033[0m"
}

function msg-info {
    echo -e "\033[34m$1\033[0m"
}

function file_check {
    if [ -z "$1" ]; then
        msg-error "Error: No filename provided!"
        return 1
    fi
    if [ ! -f "$1" ]; then
        msg-error "Error: File $1 does not exist!"
        return 1
    fi
    return 0
}

function compile_check {
    local src_file=$1
    local out_file=$2
    local extra_args=$3
    
    msg-info "Compiling $src_file..."
    g++ -std=c++17 $extra_args $src_file -o $out_file 2>&1
    if [ $? -ne 0 ]; then
        msg-error "Compilation failed for $src_file"
        return 1
    fi
    msg-success "Compilation successful: $out_file"
    return 0
}

function test_task1 {
    local src_file=$1
    local obo_file=$2
    local out_file="task1"
    
    if ! compile_check $src_file $out_file ""; then
        return 1
    fi
    
    msg-info "\n=== Testing Task 1: Basic application layout ==="
    
    # Test basic command line checks
    msg-info "Test 1.1: No arguments (should show help)"
    ./$out_file
    
    msg-info "Test 1.2: Help argument"
    ./$out_file --help
    
    msg-info "Test 1.3: Invalid command"
    ./$out_file --invalid-command $obo_file
    
    msg-info "Test 1.4: Missing filename"
    ./$out_file --consider-table
    
    msg-info "Test 1.5: Invalid namespace"
    ./$out_file --consider-table $obo_file invalid_namespace
    
    msg-info "Test 1.6: Valid command with filename"
    ./$out_file --consider-table $obo_file
    
    msg-info "Test 1.7: Valid command with filename and namespace"
    ./$out_file --consider-table $obo_file molecular_function
    
    msg-success "Task 1 tests completed (visual inspection required)"
}

function test_task2 {
    local src_file=$1
    local obo_file=$2
    local out_file="task2"
    
    if ! compile_check $src_file $out_file ""; then
        return 1
    fi
    
    msg-info "\n=== Testing Task 2: Consider table functionality ==="
    
    # Test consider table output
    msg-info "Test 2.1: Consider table for all namespaces"
    ./$out_file --consider-table $obo_file | head -n 10
    
    msg-info "Test 2.2: Consider table for molecular_function"
    ./$out_file --consider-table $obo_file molecular_function | head -n 5
    
    msg-info "Test 2.3: Check for NA values in output"
    ./$out_file --consider-table $obo_file | grep -m 3 "NA"
    
    msg-success "Task 2 tests completed (output verification required)"
}

function test_task3 {
    local src_file=$1
    local obo_file=$2
    local out_file="task3"
    
    if ! compile_check $src_file $out_file ""; then
        return 1
    fi
    
    msg-info "\n=== Testing Task 3: Advanced functionality ==="
    
    # Check for class implementation (3-1)
    msg-info "Test 3.1: Checking for class implementation (extra point)"
    grep -A 5 -E '^\s*class' $src_file
    
    # Test obsolete stats (3-2)
    msg-info "Test 3.2: Obsolete statistics"
    ./$out_file --obsolete-stats $obo_file
    
    # Test file output (3-3)
    local outfile="consider-ids.tab"
    msg-info "Test 3.3: File output with .tab extension"
    ./$out_file --consider-table $obo_file $outfile
    if [ -f $outfile ]; then
        msg-info "First 5 lines of output file:"
        head -n 5 $outfile
        rm $outfile
    else
        msg-error "Output file was not created"
    fi
    
    msg-success "Task 3 tests completed"
}

function main {
    if [ $# -lt 2 ]; then
        msg-error "Usage: $0 task-number cpp-file obo-file"
        msg-error "   or: $0 all cpp-file1 cpp-file2 cpp-file3 obo-file"
        return 1
    fi
    
    local task=$1
    shift
    
    if [ "$task" == "all" ]; then
        if [ $# -ne 4 ]; then
            msg-error "For 'all' mode, provide 3 cpp files and 1 obo file"
            return 1
        fi
        local cpp1=$1
        local cpp2=$2
        local cpp3=$3
        local obo=$4
        
        test_task1 $cpp1 $obo
        test_task2 $cpp2 $obo
        test_task3 $cpp3 $obo
    else
        if [ $# -ne 2 ]; then
            msg-error "For single task mode, provide 1 cpp file and 1 obo file"
            return 1
        fi
        local cpp=$1
        local obo=$2
        
        case $task in
            1) test_task1 $cpp $obo ;;
            2) test_task2 $cpp $obo ;;
            3) test_task3 $cpp $obo ;;
            *) msg-error "Invalid task number. Use 1, 2, 3 or 'all'" ;;
        esac
    fi
}

main "$@"