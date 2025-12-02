# Stack ADT

A generic stack Abstract Data Type (ADT) implementation in C with student data structures demonstrating polymorphic operations through function pointers.

## Overview

This project implements a type-safe, generic stack data structure that can store any element type. The implementation uses function pointers to provide polymorphic operations (clone, destroy, print) for different data types.

The project includes:
- Generic stack implementation
- Student data type as example usage
- Function pointer-based polymorphism
- Dynamic memory management

## Files

- **`stack.c`** - Stack ADT implementation
- **`stack.h`** - Stack interface and type definitions
- **`student.c`** - Student structure implementation
- **`student.h`** - Student structure interface
- **`makefile`** - Build configuration

## Architecture

### Generic Stack Design

The stack uses `void*` pointers to store elements of any type, with function pointers providing type-specific operations:

```c
typedef void *elem_t;                    // Generic element type
typedef elem_t (*clone_t)(elem_t e);     // Clone function pointer
typedef void (*destroy_t)(elem_t e);     // Destroy function pointer
typedef void (*print_t)(elem_t e);       // Print function pointer
```

### Stack Structure

```c
struct stack {
    elem_t *elements;        // Array of elements
    size_t size;             // Current number of elements
    size_t capacity;         // Maximum capacity
    clone_t clone_func;      // Function to clone elements
    destroy_t destroy_func;  // Function to destroy elements
    print_t print_func;      // Function to print elements
};
```

## How to Compile

### Using Makefile

```bash
make
```

### Manual Compilation

```bash
gcc -Wall -Wextra -Werror -std=c99 -c stack.c
gcc -Wall -Wextra -Werror -std=c99 -c student.c
gcc -Wall -Wextra -Werror -std=c99 -o stack_demo stack.o student.o main.c
```

## Stack API

### Creating a Stack

```c
pstack_t stack_create(size_t max_num_of_elem,
                      clone_t clone_func,
                      destroy_t destroy_func,
                      print_t print_func);
```

**Parameters:**
- `max_num_of_elem` - Maximum capacity of the stack
- `clone_func` - Function to clone elements (deep copy)
- `destroy_func` - Function to free element memory
- `print_func` - Function to print elements

**Returns:** Pointer to new stack, or `NULL` on allocation failure

### Stack Operations

#### Push Element
```c
Result stack_push(pstack_t stack, elem_t element);
```
- Adds element to top of stack
- Returns `SUCCESS` or `FAIL`

#### Pop Element
```c
elem_t stack_pop(pstack_t stack);
```
- Removes and returns top element
- Returns `NULL` if stack is empty
- **Important:** Caller is responsible for freeing returned element

#### Peek Element
```c
elem_t stack_peek(pstack_t stack);
```
- Returns top element without removing it
- Returns `NULL` if stack is empty
- Element remains owned by stack

#### Destroy Stack
```c
Result stack_destroy(pstack_t stack);
```
- Frees all elements using destroy function
- Frees stack memory
- Returns `SUCCESS`

### Query Operations

```c
size_t stack_size(pstack_t stack);        // Current number of elements
size_t stack_capacity(pstack_t stack);     // Maximum capacity
bool stack_is_empty(pstack_t stack);       // Check if empty
void stack_print(pstack_t stack);          // Print all elements
```

## Student Data Type

The student structure demonstrates using the generic stack:

```c
typedef struct student {
    char *name;    // Student name (dynamically allocated)
    int age;       // Student age
    int id;        // Student ID
} student_t, *pstudent_t;
```

### Student Functions

```c
elem_t student_clone(elem_t e);        // Deep copy of student
void student_destroy(elem_t e);         // Free student memory
void student_print(elem_t e);           // Print student information
```

## Usage Examples

### Example 1: Basic Stack Operations

```c
#include "stack.h"
#include "student.h"

int main() {
    // Create stack for students with capacity of 10
    pstack_t stack = stack_create(10, 
                                  student_clone,
                                  student_destroy,
                                  student_print);
    
    if (!stack) {
        return 1;  // Allocation failed
    }
    
    // Create a student
    student_t student1 = {
        .name = "Alice",
        .age = 20,
        .id = 12345
    };
    
    // Push student onto stack
    if (stack_push(stack, &student1) == FAIL) {
        printf("Stack is full!\n");
    }
    
    // Check size
    printf("Stack size: %zu\n", stack_size(stack));
    
    // Peek at top
    student_t *top = (student_t*)stack_peek(stack);
    if (top) {
        printf("Top student: %s\n", top->name);
    }
    
    // Print entire stack
    stack_print(stack);
    
    // Pop and free
    student_t *popped = (student_t*)stack_pop(stack);
    if (popped) {
        student_destroy(popped);
    }
    
    // Cleanup
    stack_destroy(stack);
    
    return 0;
}
```

### Example 2: Multiple Students

```c
pstack_t stack = stack_create(5, student_clone, 
                              student_destroy, student_print);

// Push multiple students
student_t students[] = {
    {"Alice", 20, 12345},
    {"Bob", 21, 12346},
    {"Charlie", 22, 12347}
};

for (int i = 0; i < 3; i++) {
    stack_push(stack, &students[i]);
}

// Process in LIFO order
while (!stack_is_empty(stack)) {
    student_t *s = (student_t*)stack_pop(stack);
    printf("Processing: %s\n", s->name);
    student_destroy(s);
}

stack_destroy(stack);
```

### Example 3: Check Capacity

```c
pstack_t stack = stack_create(3, student_clone,
                              student_destroy, student_print);

student_t student = {"Test", 20, 99999};

// Fill the stack
for (int i = 0; i < 5; i++) {
    if (stack_size(stack) >= stack_capacity(stack)) {
        printf("Stack is full! Cannot add more elements.\n");
        break;
    }
    stack_push(stack, &student);
}

stack_destroy(stack);
```

## Memory Management

### Clone Function
The clone function creates a **deep copy** of elements:
```c
elem_t student_clone(elem_t e) {
    pstudent_t orig = (pstudent_t)e;
    pstudent_t clone = malloc(sizeof(student_t));
    
    clone->age = orig->age;
    clone->id = orig->id;
    clone->name = malloc(strlen(orig->name) + 1);
    strcpy(clone->name, orig->name);
    
    return clone;
}
```

### Destroy Function
The destroy function frees all allocated memory:
```c
void student_destroy(elem_t e) {
    pstudent_t student = (pstudent_t)e;
    free(student->name);  // Free string first
    free(student);        // Then free struct
}
```

### Important Rules
1. ✅ `stack_push()` clones the element (stack owns a copy)
2. ✅ `stack_pop()` returns element ownership to caller
3. ✅ `stack_peek()` keeps ownership with stack
4. ✅ `stack_destroy()` frees all remaining elements
5. ⚠️ Caller must free elements returned by `stack_pop()`

## Design Principles

### Type Safety
- Uses `void*` for generic storage
- Type casting required when retrieving elements
- Function pointers ensure correct type operations

### Encapsulation
- Stack implementation hidden in `.c` file
- Access only through public API functions
- Internal structure opaque to users

### Polymorphism
- Function pointers enable type-specific operations
- Same stack implementation works for any data type
- Easy to create stacks of different element types

### LIFO Semantics
- Last In, First Out ordering
- Push adds to top
- Pop removes from top

## Error Handling

### Return Values
- `SUCCESS` (0) - Operation completed successfully
- `FAIL` (non-zero) - Operation failed
- `NULL` - Invalid operation or empty stack

### Common Errors
```c
// Stack full
if (stack_push(stack, elem) == FAIL) {
    printf("Error: Stack is full\n");
}

// Stack empty
elem_t elem = stack_pop(stack);
if (elem == NULL) {
    printf("Error: Stack is empty\n");
}

// Allocation failure
pstack_t stack = stack_create(10, clone, destroy, print);
if (stack == NULL) {
    printf("Error: Could not allocate stack\n");
}
```

## Advanced Usage

### Using Stack with Different Types

The stack can be used with any data type by providing appropriate functions:

```c
// Integer stack example
elem_t int_clone(elem_t e) {
    int *orig = (int*)e;
    int *copy = malloc(sizeof(int));
    *copy = *orig;
    return copy;
}

void int_destroy(elem_t e) {
    free(e);
}

void int_print(elem_t e) {
    printf("%d\n", *(int*)e);
}

// Create integer stack
pstack_t int_stack = stack_create(20, int_clone, 
                                  int_destroy, int_print);
```

## Performance Characteristics

- **Push**: O(1) - Constant time
- **Pop**: O(1) - Constant time  
- **Peek**: O(1) - Constant time
- **Size**: O(1) - Constant time
- **Is Empty**: O(1) - Constant time
- **Space**: O(n) - Linear in number of elements

## Limitations

- Fixed capacity (set at creation time)
- No dynamic resizing
- Sequential storage (array-based)
- Type casting required for element access

## Building Tips

### Compilation Flags
```bash
-Wall          # All warnings
-Wextra        # Extra warnings
-Werror        # Treat warnings as errors
-std=c99       # C99 standard
```

### Common Linker Issues
Make sure to link both object files:
```bash
gcc -o program stack.o student.o main.o
```

## Dependencies

- Standard C library
  - `stdlib.h` - Memory allocation
  - `stdio.h` - I/O operations
  - `stddef.h` - size_t type
  - `stdbool.h` - bool type

## Testing Recommendations

1. **Boundary Tests**
   - Push to full capacity
   - Pop from empty stack
   - Single element operations

2. **Memory Tests**
   - Use valgrind to check for leaks
   - Verify proper cleanup
   - Test clone and destroy functions

3. **Functional Tests**
   - LIFO ordering verification
   - Multiple push/pop cycles
   - Print functionality

```bash
# Memory leak check
valgrind --leak-check=full ./stack_demo
```

## Common Pitfalls

❌ **Forgetting to free popped elements**
```c
elem_t e = stack_pop(stack);
// Use e...
// Missing: student_destroy(e);  ← Memory leak!
```

✅ **Correct usage**
```c
elem_t e = stack_pop(stack);
// Use e...
student_destroy(e);  // Always free!
```

❌ **Modifying peeked elements**
```c
student_t *s = stack_peek(stack);
s->age = 25;  // ← Modifies stack content!
```

✅ **Clone if you need to modify**
```c
student_t *s = stack_peek(stack);
student_t *copy = student_clone(s);
copy->age = 25;  // Safe to modify copy
```

## Summary

This stack implementation demonstrates:
- ✅ Generic programming in C
- ✅ Function pointers for polymorphism
- ✅ Proper memory management
- ✅ Clean API design
- ✅ Type-safe operations through conventions
- ✅ Reusable data structure

The stack ADT can be easily adapted for any data type by providing appropriate clone, destroy, and print functions.
