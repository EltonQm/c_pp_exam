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
    ## send stderr to stdout to avoid 
    ## chaos messages of students
    $@ 2>&1
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
    if [ -z $1 ]; then
        msg-warn "Usage: bash $0 cpp-file ?-lfmt or other flags? | less -r"
        msg-warn "       sars-cov-files and some README.md must be in the current folder"
        return 0
    else
       if [ -f task ]; then rm task ; fi
       if [ -f out.tab ]; then rm out.tab ; fi
       msg-yellow compile       
       ## second argument might be additional flags or files to compile such as pex.cpp
       mexec  g++ -std=c++17 $1 --max-errors=3 $2 -o task "compilation"
       if ! [ -f "task" ] ; then 
           msg-error "Compilation does not worked!"
           return 1
       fi
       msg-yellow task1
       ### task 1
       mexec ./task "check usage"
       mexec ./task --help "check help"
       mexec ./task --diff dummy.fasta "check wrong file or insufficient args"
       mexec ./task --diff sars-cov1.fasta "check missing second file"
       mexec ./task --diff sars-cov1A.fasta dummy.fasta "check file not exists"              
       mexec ./task --diff sars-cov1A.fasta sars-cov1B.fasta "check all fine --diff"       
       mexec ./task --get-entry sars-cov1.fasta  "check missing ID as argument"
       mexec ./task --get-entry id1 sars-cov1.fasta  "check all fine --get-entry one file"
       mexec ./task --get-entry id1 sars-cov1.fasta  sars-cov1B.fasta "check all fine --get-entry two files"       
       msg-yellow task2
       ### task 2
       mexec ./task "check usage again"
       mexec ./task --help "check help again"
       mexec ./task --diff sars-cov1A.fasta sars-cov1B.fasta "--diff with three ids as result"
       mexec ./task --diff sars-cov1B.fasta sars-cov1A.fasta "--diff with one  ID as result"       
       ### task 3
       msg-yellow "task3.1 - class implementation"
       mexec grep -A 5 --color class $1 "check for class - task 3 (1)"
       msg-yellow "task3.2 - check for FASTA header"
       mexec ./task --get-entry 'sp|Q7TLC7|Y14_CVHSA' README.md "check README.md not FASTA I"
       mexec ./task 'sp|Q7TLC7|Y14_CVHSA' README.md "check README.md not FASTA II"       
       msg-yellow "task3.3 - check multiple files"
       mexec ./task --get-entry 'sp|Q7TLC7|Y14_CVHSA' sars-cov1A.fasta "check single file I" 
       mexec ./task --get-entry 'sp\|Q7TLC7\|Y14_CVHSA' sars-cov1A.fasta "check single file I"        
       mexec ./task 'sp|Q7TLC7|Y14_CVHSA' sars-cov1A.fasta "check single file II"        
       mexec ./task --get-entry 'sp|Q7TLC7|Y14_CVHSA' *.fasta "check multiple files I" 
       mexec ./task 'sp|Q7TLC7|Y14_CVHSA' *.fasta "check multiple files II"        
    fi
    
}

tasks $@ 
