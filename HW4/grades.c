#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grades.h"
#include "linked-list.h"

enum { SUCCESS=0, ERROR=1 };


/* ------------------		structures		------------------ */

struct grades {
	struct list *students;
};

/* opaque to the user */

struct student {
	char *name;
	int id;
	struct list *courses;
};

struct course {
	char *name;
	int grade;
};


/* ------------------		functions		------------------ */

/**
 * @brief User function, clones “element” to “output”
 * @param element the course we want to clone
 * @param output the new clone course
 * @returns 0 on success and 1 otherwise
 */
int clone_course(void *element, void **output) {

	struct course *src = (struct course*)element;
	struct course *dst = (struct course*)malloc(sizeof(struct course));
	if (!dst) {
		return ERROR;
	}

	/* copy student's grade in this course */
	dst->grade = src->grade;

	/* copy course's name */
	dst->name = malloc(sizeof(src->name));
	if (!(dst->name)) {
		free(dst);
		return ERROR;
	}
	strcpy(dst->name, src->name);

	*output = dst;
	return SUCCESS;
}


/**
 * @brief User function, destroys “element”
 * @param element the course we want to destroy
 * @note always succeed
 */
void destroy_course(void *element) {

	struct course *course = (struct course*)element;
	free(course->name);
	free(course);
}


/**
 * @brief User function, clones “element” to “output”
 * @param element the student we want to clone
 * @param output the new clone student
 * @returns 0 on success and 1 otherwise
 */
int clone_student(void *element, void **output) {

	struct student *src = (struct student*)element;
	struct student *dst = (struct student*)malloc(sizeof(struct student));
	if (!dst) {
		return ERROR;
	}

	/* copy student's id */
	dst->id = src->id;

	/* copy student's name */
	dst->name = malloc(sizeof(src->name));
	if (!(dst->name)) {
		free(dst);
		return ERROR;
	}
	strcpy(dst->name, src->name);

	/* copy student's course list */
	element_clone_t course_clone = clone_course;
	element_destroy_t course_destroy = destroy_course;
	struct list *courses= list_init(course_clone, course_destroy);
	if (!courses) {
		free(dst->name);
		free(dst);
		return ERROR;
	}
	dst->courses = courses;

	*output = dst;
	return SUCCESS;
}


/**
 * @brief User function, destroys “element”
 * @param element the student we want to destroy
 * @note always succeed
 */
void destroy_student(void *element) {

	struct student *student = (struct student*)element;
	list_destroy(student->courses);
	free(student->name);
	free(student);
}


/**
 * @brief Initializes the "grades" data-structure
 * @returns A pointer to the data-structure, or NULL in case of an error
 */
struct grades* grades_init() {

	struct grades *grades;
	grades = (struct grades*)malloc(sizeof(struct grades));
	if (!grades) {
		return NULL;
	}

	/* create empty student list */
	element_clone_t student_clone = clone_student;
	element_destroy_t student_destroy = destroy_student;
	grades->students = list_init(student_clone, student_destroy);

	return grades;
}


/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades) {

	list_destroy(grades->students);
	free(grades);
}


/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or a student with
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id) {

	/* check if student already exists */
	struct iterator *iterator = list_begin(grades->students);
	struct student *cur_student;
	while(cur_student) {
		cur_student = list_get(iterator);
		if (cur_student->id == id) {
			return ERROR;
		}
		iterator = list_next(iterator);
	}

	/* create student node */
	struct student *student;
	student = (struct student*)malloc(sizeof(struct student));
	if (!student) {
		free(student);
		return ERROR;
	}

	/* set student's id */
	student->id = id;

	/* set student's name */
	student->name = (char*)malloc(sizeof(name));
	if (!(student->name)) {
		free(student);
		return ERROR;
	}
	strcpy(student->mane, name);

	/* create empty course list */
	element_clone_t course_clone = clone_course;
	element_destroy_t course_destroy = destroy_course;
	struct list *courses= list_init(course_clone, course_destroy);
	if (!courses) {
		free(student->name);
		free(student);
		return ERROR;
	}
	student->courses = courses;

	/* push the student to the grade list */
	if ((list_push_back(grades->students, student))) {
		free(student->name);
		list_destroy(student->courses);
		free(student);
		return ERROR;
	}
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
