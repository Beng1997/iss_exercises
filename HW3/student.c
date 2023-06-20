#include "student.h"
#include <string.h>


elem_t student_clone(elem_t e) {
    if (e == NULL) {
        return NULL;
    }
    student_t *s = (student_t*) e;
    student_t *clone = malloc(sizeof(student_t));
    if (clone == NULL) {
        return NULL;
    }
    clone->name = malloc(strlen(s->name) + 1);
    if (clone->name == NULL) {
        free(clone);
        return NULL;
    }
    strcpy(clone->name, s->name);
    clone->age = s->age;
    clone->id = s->id;
    return clone;
}

void student_destroy(elem_t e) {
        if (e == NULL){
        return;
    }
    student_t *s = (student_t*) e;
    free(s->name);
    free(s);
}

void student_print(elem_t e) {
    if (e == NULL)
    {
        return;
    }
    
    student_t *s = (student_t*) e;
    printf("student name: %s, ", s->name);
    printf("age: %d, ", s->age);
    printf("id: %d.\n", s->id);
}
