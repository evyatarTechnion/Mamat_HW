#!/bin/bash

read course

if [ -z "$course" ] || ! [[ "$course" =~ ([0-9][0-9][0-9][0-9][0-9][0-9]) ]] ; then
	echo 'Wrong number of arguments'
	exit 0
fi

course_txt=$course'.txt'

if ! [[ -e $course_txt ]]; then
	echo 'Course not found'
	exit 0
fi

course_dir=$course'_stat'

if [[ -d $course_dir ]]; then
	rm -r $course_dir
fi

mkdir $course_dir

cat $course_txt | grep -E [0-9] | awk '{print $2}' > $course_dir/grades.txt

gcc -c -Wall calc_statistics.c -o prog.exe

cat calc_statistics.txt