#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int count = 0;  // number of valid grades in the file
    // count the number of valid grades in the file
    int line_num = 1; // line number counter
    int grade;
    int return_value;
    double average=0;
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
        average+=grade;
        line_num++;
        count++;
    }
    printf("%.2f\n",average/count); 
}
