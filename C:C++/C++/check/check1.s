function file_check () {
    # Check if an argument was provided
    if [ -z "$1" ]; then
        msg-errord "Error: No filename provided and/or extension provided!"
        return 1
    fi
    if [ -f "$1" ]; then
        filename=$(basename -- "$1")
        extension="${filename##*.}"
        extension=$(echo "$extension" | awk '{print tolower($0)}') ;#'
        extension=".$extension"
        givenext=$(echo "$2" | awk '{print tolower($0)}') ;#'
        if [ -z "$2" ]; then
            # no extension given, file exists
            return 0
        elif [ "$extension" == "$givenext" ]; then
            return 0
        else
            msg-error "Error: $1 has not the required extension $2 ('$extension' != '$givenext')"
            return 1
        fi
    fi
    msg-error "Error: that should not be reached!"
    return 1
}
  
function mexec {
    ## last argument
    msg=${!#}
    ## remove it from compilation
    local args=("$@")
    set -- "${args[@]:0:$((${#args[@]}-1))}"
    echo -e "\033[32m$@ ($msg)\033[0m"
    $@
}

function msg-error {
    echo -e "\033[31m$1\033[0m"
}    
    
function msg-warn  {
    echo -e "\033[33m$1\033[0m"
}
function msg-yellow  {
    echo -e "\033[33m$1\033[0m"
}
function tasks {
    if [ -z $3 ]; then
        msg-warn "Usage: bash $0 cpp-file fasta-file1 fasta-file2"
        return 0
    else
       if [ -f task ]; then rm task ; fi
       msg-yellow compile       
       ## fourth argument might be additional files to compile such as pex.cpp
       mexec  g++ -std=c++17 $4 $1 -o task "compilation"
       if ! [ -f "task" ] ; then 
           msg-error "Compilation does not worked!"
           return 1
       fi
       msg-yellow task1
       ### task 1
       mexec ./task "check usage"
       mexec ./task --help "check help"
       mexec ./task --search MA dummy.fasta "check wrong file"
       mexec ./task --dummy MA $2 "check wrong argument --dummy"
       mexec ./task --search $2 "check --search missing pattern"       
       mexec ./task --summary MA $2 "check --summary wrong argument"
       msg-yellow task2
       ### task 2
       mexec ./task --search MA $2  "check --search OK - task 2" | head -n 10
       mexec ./task --search MSD $2  "check --search OK - task 2" | head -n 10
       ### task 3
       msg-yellow task3
       mexec grep --color class $1 "check for class - task 3"
       msg-yellow "search first file"
       mexec ./task --search MSD $2 $3 "check --search OK - task 3" | head -n 10
       msg-yellow "search second file"       
       mexec ./task --search MSD $2 $3 "check --search OK - task 3" | tail -n 10       
       mexec ./task --summary $2 "check --summary OK - task 3" | head -n 10
       mexec ./task --summary $2 "check --summary check 76 length - task 3"  | grep -E '76|VEMP_CVHSA'

    fi
    
}

tasks $@ 
