#include "stack.h"

struct stack {
    size_t max_size;
    size_t size;
    elem_t *elements;
    clone_t clone_func;
    destroy_t destroy_func;
    print_t print_func;

};

/* create a new stack with a given maximum number of elements,
   clone function and destroy function */
pstack_t stack_create(size_t max_num_of_elem,
 clone_t clone_func, destroy_t destroy_func, print_t print_func) {
    pstack_t new_stack = malloc(sizeof(struct stack));
    if (new_stack == NULL) {
        return NULL;
    }
    new_stack->max_size = max_num_of_elem;
    new_stack->size = 0;
    new_stack->elements = malloc(max_num_of_elem * sizeof(elem_t));
    if (new_stack->elements == NULL) {
        free(new_stack);
        return NULL;
    }
    new_stack->clone_func = clone_func;
    new_stack->destroy_func = destroy_func;
    new_stack->print_func = print_func;
    return new_stack;
}

/* destroy a stack and all its elements */
Result stack_destroy(pstack_t stack) {
    if (stack == NULL) {
        return FAIL;
    }
    for (size_t i = 0; i < stack->size; i++) {
        stack->destroy_func(stack->elements[i]);
    }
    free(stack->elements);
    free(stack);
    return SUCCESS;
}

/* push an element onto the stack */
Result stack_push(pstack_t stack, elem_t element) {
    if (stack == NULL || stack->size == stack->max_size ) {
        return FAIL;
    }
    stack->elements[stack->size] = stack->clone_func(element);
    if(stack->elements[stack->size] == NULL) {
        return FAIL;
    }
    stack->size++;
    return SUCCESS;
}

/* pop an element from the stack */
elem_t stack_pop(pstack_t stack) {
    if (stack == NULL || stack->size == 0) {
        return NULL;
    }
    stack->size--;
    return stack->elements[stack->size];
}

/* get the top element of the stack without removing it */
elem_t stack_peek(pstack_t stack) {
    if (stack == NULL) {
        return NULL;
    }
    if (stack->size == 0) {
        return NULL;
    }
    return stack->elements[stack->size - 1];
}



/* get the size of the stack */
size_t stack_size(pstack_t stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size;
}

/* check if the stack is empty */
bool stack_is_empty(pstack_t stack) {
    // fixed condition to check if stack is empty
    if (stack == NULL || stack->size != 0) { 
        return false;
    }
    return true;
}

/* get the current capacity of the stack */
size_t stack_capacity(pstack_t stack) {
    if (stack == NULL || stack->size == 0) {
        return 0;
    }
    return stack->max_size - stack->size;
}


/* print the elements of the stack */
void stack_print(pstack_t stack) {
    if (stack == NULL) {
        printf("Stack is NULL\n");
        return;
    }
    for (size_t i = stack->size; i > 0 ; i--) {
        stack->print_func(stack->elements[i-1]); 
        // replaced print_func with stack->print_func
    }
}
