#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grades.h"
#include "linked-list.h"

enum { SUCCESS=0, ERROR=1 };


/* ------------------		structures		------------------ */

struct grades {
	struct list *students_list;
};

/* opaque to the user */

struct student {
	char *name;
	int id;
	struct list *courses_list;
};

struct course {
	char *name;
	int grade;
};


/* ------------------		functions		------------------ */

/**
 * @brief Initializes the "grades" data-structure
 * @returns A pointer to the data-structure, or NULL in case of an error
 */
struct grades* grades_init() {

	struct grades *grades;
	grades = (struct grades*)malloc(sizeof(struct grades));
	if (!grades) {
		free(grades);
		return NULL;
	}

	element_clone_t student_clone=clone_student;
	element_destroy_t student_destroy=destroy_student;

	grades->students_list = list_init(student_clone,student_destroy);

	return grades;
}


/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades) {

}


/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or a student with
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id) {

	struct student *student;
	student = (struct student*)malloc(sizeof(struct student));
	if (!student) {
		return ERROR;
	}

	student->id = id;

	student->name = (char*)malloc(sizeof(name));
	if (!(student->name)) {
		return ERROR;
	}
	strcpy(student->mane, name);
}


/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success
 * @note Fails if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade);

/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
float grades_calc_avg(struct grades *grades, int id, char **out);

/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id);

/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order
 * in which they were inserted into "grades"
 */
int grades_print_all(struct grades *grades);
