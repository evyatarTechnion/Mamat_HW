#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Integer constants */
enum { MAX_GRADE=100, PASS_GRADE=55, MIN_GRADE=1,
	 CENTUM=100, TEN=10 , MAX_CHAR=100};

/**
 * @brief Application's main entry point
 * @param argc Number of arguments
 * @param argv Pointer to arguments
 * @return Non-zero value in case of an error
 */

int main(int argc, char *argv[]) {
    if (argc < 1) {
        fprintf(stderr, "Error\n");
        return 1;
    }

	/* create files' paths */
    char grades_path[MAX_CHAR] = "./";
    strcat(grades_path, argv[1]);
   	strcat(grades_path, "_stats/grades.txt");
	char stats_path[MAX_CHAR] = "./";
	strcat(stats_path, argv[1]);
	strcat(stats_path, "_stats/course_statistics.txt");

	/* import files to read and write */
	FILE *grades, *stats;
	grades = fopen(grades_path, "r");
	stats = fopen(stats_path, "w");

	int count_grades[MAX_GRADE] = {0};
	int count_students = 0;
	int grades_sum = 0;
	int max_grade = MIN_GRADE, min_grade = MAX_GRADE;
	int passed = 0;

	/* scan grades and extract data */
	int cur_grade = 0;
	while (fscanf(grades, "%d", &cur_grade) != EOF) {
		count_students++;
		count_grades[cur_grade-1]++;
		grades_sum += cur_grade;
		if (cur_grade >= PASS_GRADE) {
			passed++;
		}
		if (cur_grade > max_grade) {
			max_grade = cur_grade;
		}
		if (cur_grade <= min_grade) {
			min_grade = cur_grade;
		}
	}

	/* calculate average grade and pass rate */
	double avg = (double)grades_sum/(double)count_students;
	double pass_rate  = ((double)passed/(double)count_students)*CENTUM;

	/* calculate median grade */
	int median_position = (count_students+1)/2;
	int count_low_grade_students = 0;
	cur_grade = MIN_GRADE - 1;
	while (count_low_grade_students < median_position) {
		cur_grade++;
		count_low_grade_students += count_grades[cur_grade-1];
	}
	int median = cur_grade;

	/* print statistics */
	fprintf(stats, "num of students = %d\n", count_students);
	fprintf(stats, "avg = %.3f\n", avg);
	fprintf(stats, "the median is - %d\n", median);
	fprintf(stats, "max grade = %d, min grade = %d\n", max_grade, min_grade);
	fprintf(stats, "pass rate = %.2f%%\n", pass_rate);

	/* print grades histogram */
	cur_grade = MIN_GRADE;
	for (int i = 0; i < TEN; i++) {
		for (int j = 0; j < TEN-1; j++) {
			fprintf(stats, "%d ",count_grades[cur_grade-1]);
			cur_grade++;
		}
		fprintf(stats, "%d\n",count_grades[cur_grade-1]);
		cur_grade++;
	}

	/* close all files */
	fclose(stats);
	fclose(grades);

	return 0;
}
