#!/bin/bash

##### constants
FILENAME="spm.sh"
TITLE="Snooker Player Profile Manager v0.1"
DB_PATH="database/snooker.db"
BACKUP="database/backup.sql"
DB_DEF="database/ddl.sql"
DOC_PATH="doc/usage.txt"

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

function backup
{
    sqlite3 -line $DB_PATH .dump > $BACKUP
}

function restore
{
    if [ ! -f $BACKUP ]
    then
        echo "File $BACKUP does not exist."
        echo "Try spm.sh --help"
        exit
    fi
    rm $DB_PATH
    sqlite3 $DB_PATH < $BACKUP
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
    sqlite3 --interactive -echo $DB_PATH
}

function syncdb
{
    if [ ! -f $DB_DEF ]
    then
        echo "File $BACKUP does not exist."
        echo "Try spm.sh --help"
        exit
    fi
    rm $DB_PATH
    sqlite3 $DB_PATH < $DB_DEF

}

function dbdump
{
    sqlite3 -line $DB_PATH .dump
}

function usage
{
    echo $TITLE
    if [ ! -f $DOC_PATH ]
    then
        echo "Error: doc/usage.txt missing."
        exit
    fi
    source $DOC_PATH
}

function spm_main
{
    case $1 in
        -b | --build | build)
            build
            ;;
        --rebuild | rebuild)
            rm -rf ./build
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
        --restore | restore)
            restore
            ;;
        --backup | backup)
            backup
            ;;
        --syncdb | syncdb)
            syncdb
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
