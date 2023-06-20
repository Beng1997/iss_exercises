#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRADES_RANGE 101
void operate(FILE *f);


int main(int argc, char **argv){
    FILE *f;
    if (argc== 1 || !strcasecmp(argv[1], "-")){
        f = stdin;
    } else {
        f = fopen(argv[1], "r");
    }
    if (!f)
    {
        fprintf(stderr, "File not found: \"%s\"\n", argv[1]);
        return 1;
    }
    operate(f);
}

void operate(FILE *f) {
    int grades[GRADES_RANGE] = {0}; // array of grades

    // read in the frequencies of the grades
    int grade;
    int return_value;
    int line_num = 1; // line number counter
    int total_count = 0;
    while (1) {
        return_value = fscanf(f, "%d", &grade);
        if (return_value == EOF) {
            break;
        } else if (return_value != 1) {
            fprintf(stderr, "Error on line %d: not a number\n", line_num);
            exit(1);
        } else if (grade < 0 || grade > 100) {
            fprintf(stderr, "Error on line %d: invalid grade\n", line_num);
            exit(1);
        }
        grades[grade]++;
        line_num++;
        total_count++;
    }


    // compute the index of the middle element
    double middle_index;
    if (total_count % 2 == 0) {
        middle_index = ((double) total_count / 2) + 0.5;
    } else {
        middle_index = (double) (total_count + 1) / 2;
    }

    // traverse the histogram and find the median
    int cumulative_sum = 0;
    double median = -1;
    for (int i = 0; i < GRADES_RANGE; i++) {
        cumulative_sum += grades[i];
        if (cumulative_sum >= middle_index) {
            // found the median bin
            median = i; // compute the median value
            break;
        }
    }

    if (median < 0) {
        fprintf(stderr, "Error: unable to find median\n");
        exit(1);
    }

    printf("%.0f\n", median);
}
