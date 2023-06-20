#ifndef __STACK__
#define __STACK__


#include <stddef.h>  /* size_t */
#include <stdbool.h> /* bool   */ 
#include "stdlib.h"
#include "stdio.h"

typedef enum { SUCCESS = 0, FAIL } Result; 

typedef struct stack *pstack_t;
typedef void *elem_t;
typedef elem_t (*clone_t)(elem_t e);
typedef void (*destroy_t)(elem_t e);
typedef void (*print_t)(elem_t e);

/**
 * @fn pstack_t stack_create(size_t max_num_of_elem, clone_t clone_func,
 *  destroy_t destroy_func , print_t print_func);
 * @brief creates a new stack with a given maximum number of elements, 
 * clone function, destroy function and print function 
 * @param max_num_of_elem - the maximum number of elements in the stack,
 *  clone_func - a function that clones an element
 * , destroy_func - a function that destroys an element, print_func - a function that prints an element
 * @return pointer to the new stack, NULL if allocation failed
 */
pstack_t stack_create(size_t max_num_of_elem,
 clone_t clone_func, destroy_t destroy_func , print_t print_func);
/**
 * @fn Result stack_destroy(pstack_t s);
 * @brief destroys the user_stack. free all the memory of the elements in the
 * stack. user_stack must be a stack created by StackCreate otherwise
 * undefined behavior)
 * @param stack - pointer to the stack to destroy
 * @return none
 */
Result stack_destroy(pstack_t stack);
/**
 * @fn Result stack_push(pstack_t stack, elem_t element);
 * @brief push an element onto the stack 
 * @param stack - pointer to the stack to push the element to, element - the element to push
 * @return SUCCESS if the element was pushed successfully, FAIL otherwise
 */
Result stack_push(pstack_t stack, elem_t element);

/**
 * @fn elem_t stack_pop(pstack_t stack);
 * @brief pop the top element from the stack
 * @param stack - pointer to the stack to pop the element from
 * @return the top element of the stack, NULL if the stack is empty
 */ 
elem_t stack_pop(pstack_t stack);


/**
 * @fn elem_t stack_peek(pstack_t stack);
 * @brief peek the top element from the stack
 * @param stack - pointer to the stack to peek the element from
 * @return the top element of the stack, NULL if the stack is empty
 */
elem_t stack_peek(pstack_t stack);

/**
 * @fn size_t stack_size(pstack_t stack);
 * @brief returns the number of elements in the stack
 * @param stack - pointer to the stack to get the size of
 * @return the number of elements in the stack
 */
size_t stack_size(pstack_t stack);

/**
 * @fn bool stack_is_empty(pstack_t stack);
 * @brief checks if the stack is empty
 * @param stack - pointer to the stack to check
 * @return true if the stack is empty, false otherwise
 */
bool stack_is_empty(pstack_t stack);

/**
 * @fn size_t stack_capacity(pstack_t stack);
 * @brief returns the maximum number of elements in the stack
 * @param stack - pointer to the stack to get the capacity of
 * @return the maximum number of elements in the stack
 */
size_t stack_capacity(pstack_t stack);

/**
 * @fn size_t stack_capacity(pstack_t stack);
 * @brief returns the maximum number of elements in the stack
 * @param stack - pointer to the stack to get the capacity of
 * @return the maximum number of elements in the stack
 */
void stack_print(pstack_t stack);




#endif /* __STACK__ */