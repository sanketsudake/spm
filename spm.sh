#!/bin/bash

##### constants
FILENAME="spm.sh"
TITLE="Snooker Player Profile Manager v0.1"

#### functions
function build
{
    echo "Building code..."
    mkdir -p build/
    cd build;
    cmake ..
    make
}

function run
{
    if [ -z "$1" ]
    then
        echo "Error: Missing file path."
        echo "Try spm.sh --help"
        exit
    fi
    if [ ! -f "$1" ]
    then
        echo "File \"$1\" does not exist."
        echo "Try spm.sh --help"
        exit
    fi
    build/spm $1
}

function search
{
    if [ -z "$1" ]
    then
        echo "Error: Missing search keyword."
        exit
    fi
    find . -iregex '.*\.\(cpp\|sql\|hpp\)' -exec grep -inH $1 {} \;
}

function count
{
    find . -iregex '.*\.\(cpp\|sql\|hpp\|sh\)' -exec cat {} \; | wc -l
}

function db
{
    sqlite3 --interactive -echo database/snooker.db -separator "| "
}

function dbdump
{
    sqlite3 -line database/snooker.db .dump
}

function usage
{
    echo $TITLE
    if [ ! -f "doc/usage.txt" ]
    then
        echo "Error: doc/usage.txt missing."
        exit
    fi
    cat doc/usage.txt
}

function spm_main
{
    case $1 in
        -b | --build | build)
            build
            ;;
        -r | --run | run)
            run $2
            ;;
        -s | --search | search)
            search $2
            ;;
        -c | --count | count)
            count
            ;;
        -db | --database | database)
            db
            ;;
        -dd | --dump | dump)
            dbdump
            ;;
        -h | --help | help)
            usage
            ;;
        *)
            echo "Error: Invalid input option."
            echo "Try spm.sh --help"
            ;;
    esac
}

#### Main
spm_main $*
