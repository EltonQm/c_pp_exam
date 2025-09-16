function task1 {
    echo -e "\033[32m g++ $1 -o task1 2>&1 | head -n 3\033[0m"
    g++ -std=c++17 $1 -o task1 2>&1 | head -n 5
    echo -e "\033[32m ./task1\033[0m" 
    ./task1 | head -n 5
    echo -e "\033[32m ./task1 --help\033[0m" 
    ./task1 --help | head -n 5
    echo -e "\033[32m ./task1 --dummy LACTASE-RXN $2\033[0m" 
    ./task1 --dummy $2 | head -n 5
    echo -e "\033[32m ./task1 --get-metacyc LACTASE-STP 2022-01-13-go.obo\033[0m" 
    ./task1 --get-metacyc LACTASE-STP $2 | head -n 5
    echo -e "\033[32m ./task1 --get-metacyc LACTASE-RXN dummy.obo\033[0m" 
    ./task1 --get-metacyc LACTASE-RXN dummy.obo | head -n 5
    echo -e "\033[32m ./task1 --get-metacyc LACTASE-RXN $2\033[0m" 
    ./task1 --get-metacyc LACTASE-RXN $2 | head -n 5
    echo -e "\033[32m ./task1 --tab-metacyc $2\033[0m" 
    ./task1 --tab-metacyc $2 | head -n 5

}
function task1f {
    echo -e "\033[32m g++ $1 -o task1 2>&1 | head -n 3\033[0m"
    g++ -std=c++17 $1 -o task1 2>&1 | head -n 5
    echo -e "\033[32m ./task1\033[0m" 
    ./task1 | head -n 5
    echo -e "\033[32m ./task1 --help\033[0m" 
    ./task1 --help | head -n 5
    echo -e "\033[32m ./task1 --dummy LACTASE-RXN -f $2\033[0m" 
    ./task1 --dummy -f $2 | head -n 5
    echo -e "\033[32m ./task1 --get-metacyc LACTASE-STP -f 2022-01-13-go.obo\033[0m" 
    ./task1 --get-metacyc LACTASE-STP -f $2 | head -n 5
    echo -e "\033[32m ./task1 --get-metacyc LACTASE-RXN -f dummy.obo\033[0m" 
    ./task1 --get-metacyc LACTASE-RXN -f dummy.obo | head -n 5
    echo -e "\033[32m ./task1 --get-metacyc LACTASE-RXN -f $2\033[0m" 
    ./task1 --get-metacyc LACTASE-RXN -f $2 | head -n 5
    echo -e "\033[32m ./task1 --tab-metacyc -f $2\033[0m" 
    ./task1 --tab-metacyc -f $2 | head -n 5

}
function task2 {
    echo -e "\033[32m g++ $1 -o task2 2>&1 | head -n 3\033[0m"
    g++ -std=c++17 $1 -o task2 2>&1 | head -n 5
    echo -e "\033[32m ./task2\033[0m" 
    ./task2 | head -n 5
    echo -e "\033[32m ./task2 --get-metacyc BETA-LACTAMASE-RXN $2\033[0m" 
    ./task2 --get-metacyc BETA-LACTAMASE-RXN $2 | head -n 5

}

function task2f {
    echo -e "\033[32m g++ $1 -o task2 2>&1 | head -n 3\033[0m"
    g++ -std=c++17 $1 -o task2 2>&1 | head -n 5
    echo -e "\033[32m ./task2\033[0m" 
    ./task2 | head -n 5
    echo -e "\033[32m ./task2 --get-metacyc BETA-LACTAMASE-RXN -f $2\033[0m" 
    ./task2 --get-metacyc BETA-LACTAMASE-RXN -f $2 | head -n 5

}

function task3 {
    grep -A 10 -E '^\s*class' $1
    echo -e "\033[32m g++ $1 -o task3 2>&1 | head -n 3\033[0m"
    g++ -std=c++17 $1 -o task3 2>&1 | head -n 5
    echo -e "\033[32m ./task3\033[0m" 
    ./task3 | head -n 5
    ## try out a few approaches for multiple MetaCyc ids
    echo -e "\033[32m ./task3 --get-metacyc BETA-LACTAMASE-RXN --get-metacyc LACTASE-RXN $2\033[0m" 
    ./task3 --get-metacyc BETA-LACTAMASE-RXN --get-metacyc LACTASE-RXN $2 | head -n 8
    echo -e "\033[32m ./task3 --get-metacyc BETA-LACTAMASE-RXN,LACTASE-RXN $2\033[0m" 
    ./task3 --get-metacyc BETA-LACTAMASE-RXN,LACTASE-RXN $2 | head -n 8
    echo -e "\033[32m ./task3 --get-metacyc BETA-LACTAMASE-RXN --get-metacyc LACTASE-RXN $2\033[0m" 
    ./task3 --get-metacyc BETA-LACTAMASE-RXN --get-metacyc LACTASE-RXN $2 | head -n 8
    echo -e "\033[32m ./task3 --get-metacyc BETA-LACTAMASE-RXN LACTASE-RXN $2\033[0m" 
    ./task3 --get-metacyc BETA-LACTAMASE-RXN LACTASE-RXN $2 | head -n 8
    echo -e "\033[32m ./task3 --tab-metacyc $2\033[0m" 
    ./task3 --tab-metacyc $2 | head -n 5
    echo -e "\033[32m ./task3 --tab-metacyc $2 | grep GO:0000016\033[0m" 
    ./task3 --tab-metacyc 2022-01-13-go.obo | grep GO:0000016

}

## use like this
## bash test-exam-1-check.sh task1 FILENAME
if [ -z $3 ]; then
    echo "Usage: $0 task-name c++-file obofile"
else
    $1 $2 $3
fi
