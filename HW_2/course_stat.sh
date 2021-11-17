#!/bin/bash

# check that we got only one argument
if  ! [[ $# -eq 1 ]] ; then
	echo 'Wrong number of arguments' >&2
	exit 0
fi

# check the agument format is 6 integers
if ! [[ $1 =~ ^[0-9]{6,6}$ ]]; then
	echo 'Wrong number of arguments' >&2
	exit 0
fi

course=($1)
course_txt=$course'.txt'

# checks if course file is exist
if ! [[ -e $course_txt ]]; then
	echo 'Course not found' >&2
	exit 0
fi

# creating direcory for course statistics
course_dir=$course'_stat'

if [[ -d $course_dir ]]; then
	rm -rf $course_dir
fi

mkdir $course_dir

# take only the grades column
cat $course_txt | grep -E [0-9] | awk '{print $2}' > $course_dir/grades.txt

# compiling and linking into prog.exe
gcc -g -Wall calc_statistics.c -o prog.exe

# execute the program
./prog.exe $course

# show the result to standart output
cat ./"$course_dir"/course_statistics.txt
