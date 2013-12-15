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
	if [ ! -f "$1" ]
	then
		echo "File \"$1\" does not exist."
		usage
	else
		echo "Runnning code with $1 "
		build/spm $1
	fi
}

function usage
{
	cat doc/usage.txt
}

function spm_main
{
	echo $TITLE
	if [ -n "$1" ]
	then
		if [ $1 = "--build" ];
		then
			build
		elif [ $1 = "--run" ];
		then
			if [ -n "$2" ]
			then
				run $2
			else
				echo "Error: Missing input argument."
				usage
			 fi
		else
			echo "Error: Check input option."
			usage
		fi
	else
		echo "Error: Missing input option."
		usage
	fi
}

#### Main
spm_main $*
