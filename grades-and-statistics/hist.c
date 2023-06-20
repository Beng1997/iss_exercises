#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *f; // pointer to the input file
static int nbins = 10; // number of bins for histogram

void operate(int *bins);

void parse_arg(int argc, char **argv) {
    f = stdin; // initialize the file pointer to stdin
    for (int i = 1; i < argc; ++i) { // loop through command line arguments
        if (!strcmp(argv[i], "-")) {
            f = stdin; // if the argument is "-", read from stdin
        } else if (!strcmp(argv[i], "-n_bins")) {
            nbins = (i < argc - 1  && strstr(argv[i+1],".txt") == NULL)
             ? atoi(argv[i + 1]) : 10; 
            // set number of bins
            i++;
        } else {
            f = fopen(argv[i], "r"); // open the input file
        }
    }
}

int main(int argc, char **argv) {
    parse_arg(argc, argv); // parse command line arguments
    if (!f) {
        fprintf(stderr, "File not found: \"%s\"\n", argv[1]); 
        // error message if file not found
        return 1;
    }
    int *bins = calloc(nbins, sizeof(int)); 
    // allocate memory for the bins array
    operate(bins); // perform histogram operation
    free(bins); // free the memory used by the bins array
}

void operate(int *bins) {
    int grade; // variable to store each grade read from the input file
    int line_num = 1; // line number in input file
    int return_value; // return value of fscanf()
    double pace; // width of each bin in the histogram
    while (1) { // loop until end of file is reached
        return_value = fscanf(f, "%d", &grade); 
        // read a grade from the input file
        if (return_value == EOF) { // check for end of file
            break;
        } else if (return_value != 1) { // check for invalid input
            fprintf(stderr, "Error on line %d: not a number\n", line_num); 
            // error message
            exit(1); // exit with error code
        } else if (grade < 0 || grade > 100) { // check for invalid grade
            fprintf(stderr, "Error on line %d: invalid grade\n", line_num); 
            // error message
            exit(1); // exit with error code
        }
        line_num++; // increment line number
        int n = (grade * nbins) / 100; 
        // determine the bin number for the grade
        if (grade == 100) { // special case for grade of 100
            n = nbins - 1;
        }
        bins[n]++; // increment the count for the appropriate bin
    }
    pace = 100.0 / nbins; // calculate the width of each bin
    for (int i = 0; i < nbins; ++i) { 
        // loop through bins and print out counts
        printf("%.0lf-%.0lf\t%d\n", pace * i, 
        (i == nbins - 1) ? 100.0 : (pace * (i+1) - 1), bins[i]);
    }
}
