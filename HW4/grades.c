#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grades.h"
#include "linked-list.h"

enum { SUCCESS=0, ERROR=1, MAX_GRADE=100, MIN_GRADE=0, AVG_ERROR=-1 };


/* ------------------		structures		------------------ */

struct grades {
	struct list *students;
};

struct Student {
	char *name;
	int id;
	struct list *courses;
};

struct Course {
	char *name;
	int grade;
};


/* ------------------		functions		------------------ */

/**
 * @brief clones “element” to “output”
 * @param element the course we want to clone
 * @param output the new clone course
 * @returns 0 on success and 1 otherwise
 */
int clone_course(void *element, void **output) {

	struct Course *src = (struct Course*)element;
	struct Course *dst = (struct Course*)malloc(sizeof(struct Course));
	if (!dst) {
		return ERROR;
	}

	/* copy student's grade in this course */
	dst->grade = src->grade;

	/* copy course's name */
	dst->name = (char*)malloc(sizeof(char)*(strlen(src->name)+1));
	if (!(dst->name)) {
		free(dst);
		return ERROR;
	}
	strcpy(dst->name, src->name);

	*output = dst;
	return SUCCESS;
}


/**
 * @brief destroys “element”
 * @param element the course we want to destroy
 * @note always succeed
 */
void destroy_course(void *element) {

	struct Course *course = (struct Course*)element;
	free(course->name);
	free(course);
}


/**
 * @brief clones “element” to “output”
 * @param element the student we want to clone
 * @param output the new clone student
 * @returns 0 on success and 1 otherwise
 */
int clone_student(void *element, void **output) {

	struct Student *src = (struct Student*)element;
	struct Student *dst = (struct Student*)malloc(sizeof(struct Student));
	if (!dst) {
		return ERROR;
	}

	/* copy student's id */
	dst->id = src->id;

	/* copy student's name */
	dst->name = (char*)malloc(sizeof(char)*(strlen(src->name)+1));
	if (!(dst->name)) {
		free(dst);
		return ERROR;
	}
	strcpy(dst->name, src->name);

	/* copy student's course list */
	element_clone_t course_clone = clone_course;
	element_destroy_t course_destroy = destroy_course;
	dst->courses = list_init(course_clone, course_destroy);
	if (!(dst->courses)) {
		free(dst->name);
		free(dst);
		return ERROR;
	}
	list_push_back(dst->courses, list_get(list_begin(src->courses)));

	*output = dst;
	return SUCCESS;
}


/**
 * @brief destroys “element”
 * @param element the student we want to destroy
 * @note always succeed
 */
void destroy_student(void *element) {

	struct Student *student = (struct Student*)element;
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
 * @brief find the cur_student node in grade list
 * @param list pointer to the list structure we want to search
 * @param id the id of the student we are looking for
 * @returns A pointer to the student node, or NULL if not found
 */
struct Student* find_student(struct list *list, int id) {

	struct iterator *iterator = list_begin(list);
	struct Student *cur_student;
	while(iterator) {
		cur_student = list_get(iterator);
		if (cur_student->id == id) {
			return cur_student;
		}
		iterator = list_next(iterator);
	}
	return NULL;
}


/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or a student with
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id) {

	if (!grades) {
		return ERROR;
	}

	/* check if student already exists */
	if (find_student(grades->students, id) != NULL) {
		return ERROR;
	}

	/* create student node */
	struct Student *new_student;
	new_student = (struct Student*)malloc(sizeof(struct Student));
	if (!new_student) {
		free(new_student);
		return ERROR;
	}

	/* set student's id */
	new_student->id = id;

	/* set student's name */
	new_student->name = (char*)malloc(sizeof(strlen(name)+1));
	if (!(new_student->name)) {
		free(new_student);
		return ERROR;
	}
	strcpy(new_student->name, name);

	/* create empty course list */
	element_clone_t course_clone = clone_course;
	element_destroy_t course_destroy = destroy_course;
	struct list *courses = list_init(course_clone, course_destroy);
	if (!courses) {
		free(new_student->name);
		free(new_student);
		return ERROR;
	}
	new_student->courses = courses;

	/* push the student to the grade list */
	if (list_push_back(grades->students, new_student)) {
		free(new_student->name);
		list_destroy(new_student->courses);
		free(new_student);
		return ERROR;
	}

	free(new_student->name);
	list_destroy(new_student->courses);
	free(new_student);
	return SUCCESS;
}


/**
 * @brief checks if this course exists in the course list
 * @param list pointer to the list structure we want to search
 * @param name the name of the course we are looking for
 * @return 0 on success (= course exists)
 */
int is_course_exists(struct list *list, const char *name) {

	struct iterator *iterator = list_begin(list);
	struct Course *cur_course;
	while(iterator) {
		cur_course = list_get(iterator);
		if(strcmp(cur_course->name, name) == 0) {
			return SUCCESS;
		}
		iterator = list_next(iterator);
	}
	return ERROR;
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
                     int grade) {

	if (!grades) {
		return ERROR;
	}

	/* check that the input grade is in valid range */
	if (grade < MIN_GRADE || grade > MAX_GRADE) {
		return ERROR;
	}

	/* set cur_student to the node of student with cur id */
	struct Student *cur_student;
	cur_student = find_student(grades->students,id);

	/* check if student exists in grades list */
	if (cur_student == NULL) {
		return ERROR;
	}

	/* check if the course already exist in cur_student's grade list */
	if (!(is_course_exists(cur_student->courses,name))) {
		return ERROR;
	}

	/* creating new node for cur_student's course */
	struct Course *new_course;
	new_course=(struct Course*)malloc(sizeof(struct Course));
	if (!new_course) {
		free(new_course);
		return ERROR;
	}

	/* insert course's grade */
	new_course->grade=grade;

	/* insert course's name */
	new_course->name=(char*)malloc(sizeof(char)*(strlen(name)+1));
	if (!(new_course->name)) {
		free(new_course->name);
		free(new_course);
		return ERROR;
	}
	strcpy(new_course->name, name);

	if(!(list_push_back(cur_student->courses,new_course))) {
		free(new_course->name);
		free(new_course);
		return SUCCESS;
	}
	else {
		free(new_course->name);
		free(new_course);
		return ERROR;
	}
}


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
float grades_calc_avg(struct grades *grades, int id, char **out) {

	if (!grades) {
		return AVG_ERROR;
	}

	struct Student *cur_student;
	cur_student = find_student(grades->students,id);

	/* check if student exists in the grade list */
	if (cur_student == NULL) {
		return AVG_ERROR;
	}
	
	/* copy cur_student's name to out */
	char *name = (char*)malloc(sizeof(char)*(strlen(cur_student->name)+1));
	if (!name) {
		out=NULL;
		free(name);
		return AVG_ERROR;
	}

	strcpy(name , cur_student->name);
	*out=name;

	/* set some variables for avg calculation */
	int num_of_courses = 0;
	float tot_grades = 0;
	float avg = 0;

	/* iterating on all cur_student's courses */
	struct iterator *iterator=list_begin(cur_student->courses);
	if (!iterator) {
		return avg;
	}
	
	struct Course *cur_course;
	while (iterator) {
		cur_course = list_get(iterator);
		tot_grades+=(cur_course->grade);
		num_of_courses++;
		iterator=list_next(iterator);
	}

	avg = (tot_grades/num_of_courses);
	return avg;
	
}


/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id) {

	if (!grades) {
		return ERROR;
	}

	/* set cur_student to the node of student with cur id */
	struct Student *cur_student;
	cur_student = find_student(grades->students, id);

	/* check if student exists in the grade list */
	if (cur_student == NULL) {
		return ERROR;
	}

	/* prints student name and id */
	printf("%s %d:",cur_student->name,id);

	/* initialize struct for student's course */
	struct iterator *iterator=list_begin(cur_student->courses);
	struct Course *cur_course;

	/* prints all student's courses name and grades */
	while(iterator) {
		cur_course=list_get(iterator);
		printf(" %s %d",cur_course->name,cur_course->grade);
		iterator=list_next(iterator);
		if (iterator) {
			printf(",");
		}
	}
	printf("\n");
	return SUCCESS;
}


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
int grades_print_all(struct grades *grades) {

	if(!grades) {
		return ERROR;
	}

	struct Student *cur_student;
	struct iterator *iterator = list_begin(grades->students);
	while(iterator) {
		cur_student = list_get(iterator);
		grades_print_student(grades, cur_student->id);
		iterator = list_next(iterator);
	}

	return SUCCESS;
}
