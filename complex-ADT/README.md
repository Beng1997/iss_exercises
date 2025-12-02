# Complex ADT - Grades Management System

A comprehensive grades management system implemented as an Abstract Data Type (ADT) in C, utilizing linked lists for efficient data organization.

## Overview

This project implements a complex data structure for managing student information including:
- Student records with IDs and names
- Course enrollments for each student
- Grade tracking per course
- Statistical operations on grade data

## Files

- **`grades.c`** - Main implementation of the grades ADT
- **`grades.h`** - Header file with function declarations and type definitions (required dependency)
- **`linked-list.h`** - Generic linked list implementation (required dependency)
- **`makefile`** - Standard Unix makefile for building the project
- **`Makefile.mak`** - Alternative makefile configuration

## Data Structures

### Main Structures

```c
struct grades {
    struct list *students;  // Pointer to list of students
};

struct Student {
    char *name;             // Student's name
    int id;                 // Student's ID
    struct list *courses;   // List of enrolled courses
};

struct Course {
    char *name;             // Course name
    int grade;              // Course grade (0-100)
};
```

### Architecture
- Uses a linked list to store students
- Each student has a linked list of courses
- Generic linked list implementation for flexibility
- Function pointers for clone/destroy operations

## Features

- ✅ Add/remove students
- ✅ Add/remove courses for students
- ✅ Update course grades
- ✅ Calculate student averages
- ✅ Memory management with proper cleanup
- ✅ Grade validation (0-100 range)

## How to Compile

### Using makefile

```bash
make
```

### Using Makefile.mak

```bash
make -f Makefile.mak
```

### Manual Compilation

```bash
gcc -Wall -Wextra -Werror -std=c99 -o grades grades.c -I.
```

## Usage Example

```c
#include "grades.h"

int main() {
    // Create a new grades structure
    struct grades *grades = grades_init();
    
    // Add a student
    grades_add_student(grades, "John Doe", 123456);
    
    // Add a course for the student
    grades_add_grade(grades, "John Doe", 123456, "Mathematics", 95);
    
    // Calculate average
    float avg = grades_calc_avg(grades, 123456, NULL);
    
    // Clean up
    grades_destroy(grades);
    
    return 0;
}
```

## API Operations

### Student Operations
- `grades_init()` - Initialize grades system
- `grades_destroy()` - Free all memory
- `grades_add_student()` - Add new student
- `grades_remove_student()` - Remove student and their data

### Course Operations
- `grades_add_grade()` - Add course with grade for student
- `grades_remove_grade()` - Remove a course from student
- `grades_update_grade()` - Update existing grade

### Query Operations
- `grades_calc_avg()` - Calculate student's average grade
- `grades_print_student()` - Print student information
- `grades_print_all()` - Print all students and grades

## Implementation Details

### Memory Management
- Dynamic memory allocation for strings (names)
- Linked list nodes allocated dynamically
- Clone functions for deep copying
- Destroy functions for proper cleanup

### Error Handling
- Returns `SUCCESS` (0) or `ERROR` (1) status codes
- Validates grade ranges (MIN_GRADE=0, MAX_GRADE=100)
- Returns `AVG_ERROR` (-1) for invalid average calculations

### Constraints
- Grades must be between 0 and 100
- Student IDs must be unique
- Course names are case-sensitive

## Building Tips

1. Ensure all header files are in the same directory
2. The linked-list module must be available
3. Use `-Wall -Wextra -Werror` for strict compilation
4. Compile with C99 standard or later

## Common Issues

**Linker errors**: Make sure linked-list.c is compiled and linked
```bash
gcc -c linked-list.c
gcc -c grades.c
gcc -o grades_program linked-list.o grades.o main.c
```

**Memory leaks**: Always call `grades_destroy()` before program exit

**Segmentation faults**: Check for NULL pointers before dereferencing

## Advanced Features

- Generic linked list allows reuse for other data structures
- Function pointer-based operations enable polymorphism
- Modular design separates concerns
- Extensible for additional student/course attributes

## Dependencies

- Standard C library (`stdio.h`, `stdlib.h`, `string.h`)
- Custom `linked-list.h` implementation
- POSIX-compliant system (for standard functions)
