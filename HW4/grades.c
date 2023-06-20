#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grades.h"
#include "linked-list.h"


enum { SUCCESS=0, ERROR=1, MAX_GRADE=100, MIN_GRADE=0, AVG_ERROR=-1 }; 


/* ------------------		structures		------------------ */
/**
 * @brief A struct representing a list of students
 * @param students A pointer to the list of students
 */
struct grades {
	struct list *students;
};
/**
 * @brief A struct representing a student
 * @param name A pointer to the student's name
 * @param id The student's id
 * @param courses A pointer to the list of the student's courses
 */
struct Student {
	char *name;
	int id;
	struct list *courses;
};
/**
 * @brief A struct representing a course
 * @param name A pointer to the course's name
 * @param grade The course's grade
 */
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

//
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
    
	if(!(list_push_back(dst->courses, list_get(list_begin(src->courses))))){
        destroy_student(dst);
        return  ERROR;
    };

	*output = dst;
	return SUCCESS;
}



/**
 * @brief Creates a new list
 * @param elem_clone A user function that clones elements
 * @param elem_destroy A user function that destroys elements 
 * @returns A pointer to the list, or NULL on error
 */
struct grades* grades_init() {

    struct grades *grades = (struct grades*)malloc(sizeof(struct grades));
    if (!grades) {
        return NULL;
    }

    element_clone_t student_clone = clone_student;
    element_destroy_t student_destroy = destroy_student;
    grades->students = list_init(student_clone, student_destroy);
    if (!(grades->students)) {
        free(grades);
        return NULL;
    }

    return grades;
}
/**
 * @brief Destroys "grades", de-allocate all memory!
 * @param grades the grades we want to destroy
 * @note always succeed
 * 
 */

void grades_destroy(struct grades *grades){
    list_destroy(grades->students);
    free(grades);
}
/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @param grades the grades we want to add the student to
 * @param name the name of the student
 * @param id the id of the student
 * @returns 0 on success and 1 otherwise
 * @note Failes if "grades" is invalid, or a student with 
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id){
    if (!grades || !name) {
        return ERROR;
    }

    /* check if student already exists */
    struct iterator *it = list_begin(grades->students);
    while (it) {
        struct Student *student =  (struct Student*)list_get(it)
        != NULL ? (struct Student*)list_get(it) : NULL;
        if (student->id == id) {
            return ERROR;
        }
        it = list_next(it);
    }

    /* create new student */
    struct Student *student = (struct Student*)malloc(sizeof(struct Student));
    if (!student) {
        return ERROR;
    }

    /* copy student's id */
    student->id = id;

    /* copy student's name */
    student->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    if (!(student->name)) {
        free(student);
        return ERROR;
    }
    strcpy(student->name, name);

    /* create student's course list */
    element_clone_t course_clone = clone_course;
    element_destroy_t course_destroy = destroy_course;
    student->courses = list_init(course_clone, course_destroy);
    if (!(student->courses)) {
        free(student->name);
        free(student);
        return ERROR;
    }

    /* add student to grades */
    if(!list_push_back(grades->students, student)){
        destroy_student(student);
        return  SUCCESS;}

    
    return SUCCESS;
}
/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @param grades the grades we want to add the course to
 * @param name the name of the course
 * @param id the id of the student
 * @param grade the grade of the course
 * @returns 0 on success and 1 otherwise
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
int grades_add_grade(struct grades *grades,
    const char *name, int id, int grade){
    if (!grades || !name || grade < MIN_GRADE || grade > MAX_GRADE) {
        return ERROR;
    }

    /* find student */
    struct iterator *it = list_begin(grades->students);
    while (it) {
        struct Student *student = (struct Student*)list_get(it);
        if (student->id == id) {
            /* check if course already exists */
            struct iterator *it2 = list_begin(student->courses);
            while (it2) {
                struct Course *course = (struct Course*)list_get(it2);
                if (strcmp(course->name, name) == 0) { 
                    course->grade = grade;
                    return SUCCESS;
                }
                it2 = list_next(it2);
            }
            /* add new course to student */
            struct Course *course = 
                (struct Course*)malloc(sizeof(struct Course));
            if (!course) {
                return ERROR;
            }
            course->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
            if (!(course->name)) {
                free(course);
                return ERROR;
            }
            strcpy(course->name, name);
            course->grade = grade;
            if(!list_push_back(student->courses, course)){
                destroy_course(course);
                return  SUCCESS;
            };

            return SUCCESS;
        }
        it = list_next(it);
    }
    return ERROR;
}
/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param grades the grades we want to calculate the average from
 * @param id the id of the student we want to calculate the average from
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
float grades_calc_avg(struct grades *grades, int id, char **out){
    if (!grades) {
        return AVG_ERROR;
    }

    /* find student */
    struct iterator *it = list_begin(grades->students);
    while (it) {
        struct Student *student = (struct Student*)list_get(it);
        if (student->id == id) {
            /* calculate average */
            float sum = 0;
            int count = 0;
            struct iterator *it2 = list_begin(student->courses);
            while (it2) {
                struct Course *course = (struct Course*)list_get(it2);
                sum += course->grade;
                count++;
                it2 = list_next(it2);
            }
            /* copy student's name */
            if (*out == NULL) {
                *out = (char*)malloc(sizeof(char)*(strlen(student->name)+1));
            if (*out == NULL) {
                return AVG_ERROR;
            }
            } else {
                free(*out);
                *out = (char*)malloc(sizeof(char)*(strlen(student->name)+1));
                if (*out == NULL) {
                return AVG_ERROR;
                }
            }

            strcpy(*out, student->name);
            if (count == 0) {
                return 0;
            }
            float avg = sum/count;
            return avg;
        }
        it = list_next(it);
    }

    return AVG_ERROR;
}
/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @param grades the grades we want to print the student from
 * @param id the id of the student we want to print
 * @returns 0 on success and 1 otherwise
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id){
    if (!grades) {
        return ERROR;
    }

    /* find student */
    struct iterator *it = list_begin(grades->students);
    while (it) {
        struct Student *student = (struct Student*)list_get(it);
        if (student->id == id) {
            /* print student's name and id */
            printf("%s %d:", student->name, student->id);

            /* print student's courses */
            struct iterator *it2 = list_begin(student->courses);
            while (it2) {
                struct Course *course = (struct Course*)list_get(it2);
                                if (it2 == list_end(student->courses))
                {
                    printf(" %s %d", course->name, course->grade);
                }
                else printf(" %s %d,", course->name, course->grade);
                it2 = list_next(it2);
                
            }
            printf("\n");
            return SUCCESS;
        }
        it = list_next(it);
    }

    return ERROR;
}

/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @param grades the grades we want to print all the students from
 * @returns 0 on success and 1 otherwise
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 */
int grades_print_all(struct grades *grades){
    if (!grades) {
        return ERROR;
    }

    /* print all students */
    struct iterator *it = list_begin(grades->students);
    while (it) {
        struct Student *student = (struct Student*)list_get(it);
        grades_print_student(grades, student->id);
        it = list_next(it);
    }
    
    return SUCCESS;
}                     
