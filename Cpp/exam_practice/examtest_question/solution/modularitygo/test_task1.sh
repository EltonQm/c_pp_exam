function task1 {
    echo -e "\033[32mCompiling task1...\033[0m"
    g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter task1.cpp task_utils.o -o task1 -lz 2>&1 | head -n 5

    echo -e "\033[32mRunning ./task1\033[0m"
    ./task1 | head -n 5

    echo -e "\033[32mRunning ./task1 --help\033[0m"
    ./task1 --help | head -n 5

    echo -e "\033[32mTesting unknown flag: ./task1 --dummy $2\033[0m"
    ./task1 --dummy $2 | head -n 5

    echo -e "\033[32mRunning --consider-table with GO file: ./task1 --consider-table biological_process /Users/eltonugbogu/Desktop/C_Cpp/Cpp/exam_practice/data/gene_ontology_edit_2022-01-13.go.obo\033[0m"
    ./task1 --consider-table biological_process /Users/eltonugbogu/Desktop/C_Cpp/Cpp/exam_practice/data/gene_ontology_edit_2022-01-13.go.obo | head -n 5

    echo -e "\033[32mTesting --obsolete-stats with dummy.obo\033[0m"
    ./task1 --obsolete-stats cellular_component dummy.obo | head -n 5

    echo -e "\033[32mTesting --obsolete-stats with passed file $2\033[0m"
    ./task1 --obsolete-stats $2 | head -n 5

    echo -e "\033[32mTesting --consider-table $2\033[0m"
    ./task1 --consider-table $2 | head -n 5
}


function task1f {
    echo -e "\033[32mCompiling task1...\033[0m"
    g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter task1.cpp task_utils.o -o task1 -lz 2>&1 | head -n 5

    echo -e "\033[32mRunning ./task1\033[0m"
    ./task1 | head -n 5

    echo -e "\033[32mRunning ./task1 --help\033[0m"
    ./task1 --help | head -n 5

    echo -e "\033[32mTesting unknown flag: ./task1 --dummy -f $2\033[0m"
    ./task1 --dummy -f $2 | head -n 5

    echo -e "\033[32mRunning --consider-table with biological_process and -f $2\033[0m"
    ./task1 --consider-table biological_process -f $2 | head -n 5

    echo -e "\033[32mRunning --obsolete-stats with dummy.obo using -f\033[0m"
    ./task1 --obsolete-stats cellular_component -f dummy.obo | head -n 5

    echo -e "\033[32mRunning --obsolete-stats with $2 using -f\033[0m"
    ./task1 --obsolete-stats cellular_component -f $2 | head -n 5

    echo -e "\033[32mRunning --consider-table -f $2\033[0m"
    ./task1 --consider-table -f $2 | head -n 5
}


function task2 {
    echo -e "\033[32m g++ $1 -o task2 2>&1 | head -n 3\033[0m"
    g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter task2.cpp task2_utils.o -o task2 -lz 2>&1 | head -n 5
    echo -e "\033[32m ./task2\033[0m" 
    ./task2 | head -n 5
    echo -e "\033[32m ./task2--consider-table  || --obsolete-stats molecular_function $2\033[0m" 
    ./task2--consider-table  || --obsolete-stats molecular_function $2 | head -n 5

}

function task2f {
    echo -e "\033[32m g++ $1 -o task2 2>&1 | head -n 3\033[0m"
     g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter task2.cpp task2_utils.o -o task2 -lz 2>&1 | head -n 5
    echo -e "\033[32m ./task2\033[0m" 
    ./task2 | head -n 5
    echo -e "\033[32m ./task2--consider-table  || --obsolete-stats molecular_function -f $2\033[0m" 
    ./task2--consider-table  || --obsolete-stats molecular_function -f $2 | head -n 5

}

function task3 {
    grep -A 10 -E '^\s*class' $1
    echo -e "\033[32m g++ $1 -o task3 2>&1 | head -n 3\033[0m"
     g++ -std=c++23 -Wall -Wextra -O2 -Iargparse/include -Wno-unused-parameter task3.cpp task3_utils.o -o task3 -lz 2>&1 | head -n 5
    echo -e "\033[32m ./task3\033[0m" 
    ./task3 | head -n 5
    ## try out a few approaches for multiple MetaCyc ids
    echo -e "\033[32m ./task3--consider-table  || --obsolete-stats molecular_function--consider-table  || --obsolete-stats cellular_component $2\033[0m" 
    ./task3--consider-table  || --obsolete-stats molecular_function--consider-table  || --obsolete-stats cellular_component $2 | head -n 8
    echo -e "\033[32m ./task3--consider-table  || --obsolete-stats molecular_function,cellular_component $2\033[0m" 
    ./task3--consider-table  || --obsolete-stats molecular_function,cellular_component $2 | head -n 8
    echo -e "\033[32m ./task3--consider-table  || --obsolete-stats molecular_function--consider-table  || --obsolete-stats cellular_component $2\033[0m" 
    ./task3--consider-table  || --obsolete-stats molecular_function--consider-table  || --obsolete-stats cellular_component $2 | head -n 8
    echo -e "\033[32m ./task3--consider-table  || --obsolete-stats molecular_function cellular_component $2\033[0m" 
    ./task3--consider-table  || --obsolete-stats molecular_function cellular_component $2 | head -n 8
    echo -e "\033[32m ./task3 --consider-table $2\033[0m" 
    ./task3 --consider-table $2 | head -n 5
    echo -e "\033[32m ./task3 --consider-table $2 | grep GO:0000016\033[0m" 
    ./task3 --consider-table 2022-01-13-go.obo | grep GO:0000016

}

#!/bin/bash

# Check if the third argument is missing
if [ -z "$3" ]; then
    # If missing, show correct usage format
    echo "Usage: $0 task-name cpp-file obofile [namespace]"
    echo "Example: bash $0 task1f task1.cpp dummy.obo biological_process"
else
    # If at least 3 arguments are given, call the function specified by $1
    # with $2 and $3 as arguments
    $1 "$2" "$3"
fi

