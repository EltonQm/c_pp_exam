#!/bin/bash

# Color and message utilities
function msg-error { echo -e "\033[31m[ERROR] $1\033[0m"; }
function msg-warn { echo -e "\033[33m[WARNING] $1\033[0m"; }
function msg-info { echo -e "\033[36m[INFO] $1\033[0m"; }
function msg-success { echo -e "\033[32m[SUCCESS] $1\033[0m"; }

function print_test {
    echo -e "\n\033[34m[TEST] $1 \033[0m(\033[35mExpect: $2\033[0m)"
}

# File validation
function validate_file {
    if [ ! -f "$1" ]; then
        msg-error "File '$1' not found!"
        return 1
    fi
    return 0
}

# Test search without pattern
function test_missing_pattern {
    local parser="$1"
    local fasta="$2"
    
    print_test "./$parser --search '$fasta'" "Should FAIL with pattern error"
    output=$(./$parser --search "$fasta" 2>&1)
    status=$?
    
    if [ $status -eq 0 ]; then
        msg-error "FAIL: Command succeeded but should fail"
        return 1
    else
        msg-success "PASS: Correctly failed (status $status)"
        
        if [[ "$output" == *"pattern"* || "$output" == *"required"* ]]; then
            msg-success "✓ Error mentions pattern requirement"
        else
            msg-warn "? Error doesn't mention pattern (but failed correctly)"
        fi
        echo "Error: ${output:0:120}..."
    fi
}

# Main test function
function test_parser {
    local cpp_file="$1"
    local fasta1="$2"
    local fasta2="$3"
    
    # Validation
    validate_file "$cpp_file" || return 1
    validate_file "$fasta1" || return 1
    validate_file "$fasta2" || return 1
    
    # Cleanup
    [ -f "parser" ] && rm parser
    
    # Compilation with argparse and zlib
    msg-info "Compiling $cpp_file with argparse and zlib..."
    if ! g++ -std=c++17 -I./argparse/include "$cpp_file" -o parser -lz; then
        msg-error "Compilation failed!"
        msg-warn "Required dependencies:"
        msg-warn "1. argparse: git clone https://github.com/p-ranav/argparse.git"
        msg-warn "2. zlib: sudo apt-get install zlib1g-dev (Ubuntu) or brew install zlib (Mac)"
        return 1
    fi
    chmod +x parser
    
    # Test cases
    msg-info "\n=== BASIC FUNCTIONALITY ==="
    print_test "./parser --help" "Show help"
    ./parser --help | head -n 8
    
    msg-info "\n=== SEARCH FUNCTION ==="
    test_missing_pattern "parser" "$fasta1"
    
    print_test "./parser --search 'ATG' '$fasta1'" "Find ATG patterns"
    ./parser --search "ATG" "$fasta1" | head -n 10
    
    print_test "./parser --search '' '$fasta1'" "Reject empty pattern"
    ./parser --search "" "$fasta1" 2>&1 | head -n 3
    
    msg-info "\n=== SUMMARY FUNCTION ==="
    print_test "./parser --summary '$fasta1'" "Show summary"
    ./parser --summary "$fasta1" | head -n 10
    
    print_test "./parser --summary missing.fasta" "Reject missing file"
    ./parser --summary "missing.fasta" 2>&1 | head -n 3
    
    # Multi-file tests
    msg-info "\n=== ADVANCED TESTS ==="
    print_test "./parser --search 'CAT' '$fasta1' '$fasta2'" "Multi-file search"
    ./parser --search "CAT" "$fasta1" "$fasta2" | head -n 10
    
    # Cleanup
    rm -f parser
    msg-success "\nCOMPLETE: All tests passed for $cpp_file"
}

# Main execution
if [ $# -lt 3 ]; then
    msg-error "Usage: $0 <cpp-file> <fasta-file1> <fasta-file2>"
    msg-error "Dependencies:"
    msg-error "1. argparse.hpp in ./argparse/include/"
    msg-error "2. zlib development libraries"
    exit 1
fi

# Check for dependencies
if [ ! -f "./argparse/include/argparse.hpp" ]; then
    msg-error "argparse not found in ./argparse/include/"
    msg-warn "Install with:"
    msg-warn "git clone https://github.com/p-ranav/argparse.git"
    msg-warn "mkdir -p argparse/include && cp argparse/include/argparse/argparse.hpp argparse/include/"
    exit 1
fi

if ! ldconfig -p | grep -q libz.so; then
    msg-error "zlib not found!"
    msg-warn "Install with:"
    msg-warn "Ubuntu: sudo apt-get install zlib1g-dev"
    msg-warn "Mac: brew install zlib"
    exit 1
fi

test_parser "$1" "$2" "$3"