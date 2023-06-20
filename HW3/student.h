#ifndef __STUDENT__
#define __STUDENT__

#include "stack.h"
#include "stdlib.h"

typedef struct student {
	char *name;
	int age;
    int id;
}*pstudent_t, student_t;
/**
 * @fn elem_t student_clone (elem_t e);
 * @brief clones a student
 * @param e - the student to clone
 * @return pointer to the cloned student
 */
elem_t student_clone (elem_t e);


/**
 * @fn void student_destroy(elem_t e);
 * @brief destroys a student
 * @param e - the student to destroy
 * @return none
 */
void student_destroy(elem_t e);

/**
 * @fn void student_print (elem_t e);
 * @brief prints a student
 * @param e - the student to print
 * @return none
 */
void student_print (elem_t e);


#endif /* __STUDENT__ */ 