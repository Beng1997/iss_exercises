# Grades and Statistics

A comprehensive statistical analysis toolkit for processing student grade data. This project includes multiple C programs for computing various statistics and a Bash orchestration script.

## Overview

This toolkit processes grade data from text files and generates:
- Statistical measures (mean, median, min, max)
- Grade distribution histograms
- Pass rate calculations
- Formatted summary reports

## Files

### C Programs
- **`mean.c`** - Calculates arithmetic mean of grades
- **`median.c`** - Computes median grade value
- **`min.c`** - Finds minimum grade
- **`max.c`** - Finds maximum grade
- **`hist.c`** - Generates histogram of grade distribution

### Scripts
- **`calc_stat.sh`** - Main orchestration script that runs all statistical programs

### Data Files
- **`answers.txt`** - Sample results or test answers

## How to Compile

### Compile All Programs

```bash
gcc -Wall -Wextra -std=c99 -o mean.exe mean.c
gcc -Wall -Wextra -std=c99 -o median.exe median.c
gcc -Wall -Wextra -std=c99 -o min.exe min.c
gcc -Wall -Wextra -std=c99 -o max.exe max.c
gcc -Wall -Wextra -std=c99 -o hist.exe hist.c
```

### Compile with Makefile (if present)

```bash
make
```

## Input Format

Grade files should be in plain text format with one grade per line:

```
85
92
78
95
88
73
90
```

**Requirements:**
- File name must be in format: `XXXXXX.txt` (6 digits)
- Each line contains a single integer grade
- Valid grade range: 0-100
- No empty lines or non-numeric values

## How to Use

### Individual Programs

#### Calculate Mean
```bash
./mean.exe 123456.txt
```
Output: `86.57` (average of all grades)

#### Calculate Median
```bash
./median.exe 123456.txt
```
Output: `88` (middle value when sorted)

#### Find Minimum
```bash
./min.exe 123456.txt
```
Output: `73` (lowest grade)

#### Find Maximum
```bash
./max.exe 123456.txt
```
Output: `95` (highest grade)

#### Generate Histogram
```bash
./hist.exe 123456.txt
```
Default output: 10 bins showing grade distribution
```
0-10    2
10-20   5
20-30   8
...
```

#### Custom Histogram Bins
```bash
./hist.exe 123456.txt -n_bins 20
```
Generates histogram with 20 bins instead of default 10

### Using the Orchestration Script

The `calc_stat.sh` script automates the entire analysis process.

#### Run Complete Analysis
```bash
chmod +x calc_stat.sh
./calc_stat.sh 123456
```

#### What the Script Does:
1. Validates input arguments (course ID format)
2. Checks if grade file exists
3. Creates output directory: `123456_stat/`
4. Generates histogram → `123456_stat/histogram.txt`
5. Calculates all statistics → `123456_stat/statistics.txt`
6. Computes pass rate (grades ≥ 55)
7. Cleans up temporary files

## Output Format

### statistics.txt
```
86.57	88	73	95	78%
```
Format: `mean	median	min	max	pass_rate`

### histogram.txt
```
0-10    2
10-20   5
20-30   8
30-40   12
40-50   15
50-60   18
60-70   20
70-80   22
80-90   25
90-100  18
```

## Script Usage Details

### calc_stat.sh Arguments

```bash
./calc_stat.sh <course_id>
```

**course_id**: 6-digit course identifier (e.g., 123456)

### Error Handling

The script validates:
- ✅ Correct number of arguments
- ✅ Course ID format (exactly 6 digits)
- ✅ File existence

Error messages:
```bash
Wrong number of arguments           # If not exactly 1 argument
Argument is not in xxxxxx format   # If not 6 digits
Course not found                    # If file doesn't exist
```

Exit codes:
- `0` - Success
- `1` - Error occurred

## Technical Details

### Histogram Implementation (hist.c)

**Features:**
- Configurable number of bins (default: 10)
- Automatic bin width calculation
- Handles input from file or stdin
- Error reporting for invalid grades

**Command line options:**
```bash
hist.exe <file>              # Use file input, 10 bins
hist.exe -                   # Use stdin, 10 bins
hist.exe <file> -n_bins 20   # Use file, 20 bins
```

### Pass Rate Calculation

The script calculates pass percentage:
```bash
pass_percent = (grades >= 55) / (total grades) * 100
```

Uses `awk` for efficient text processing:
```bash
# Count passing grades
pass=$(awk -F '\t' '$1 >= 55 {s+=$2} END {print s}' histogram.txt)

# Calculate percentage
pass_percent=$(awk "BEGIN {printf \"%d\", $pass/$total*100}")
```

## Advanced Usage

### Process Multiple Course Files

```bash
for course in 123456 234567 345678; do
    ./calc_stat.sh $course
done
```

### Custom Analysis Pipeline

```bash
# Generate detailed histogram with many bins
./hist.exe grades.txt -n_bins 50 > detailed_hist.txt

# Combine multiple statistics
mean=$(./mean.exe grades.txt)
median=$(./median.exe grades.txt)
echo "Mean: $mean, Median: $median"
```

## Dependencies

### Required Tools
- GCC compiler (C99 or later)
- Bash shell
- Standard Unix utilities: `awk`, `grep`, `sed`, `cut`

### System Requirements
- POSIX-compliant operating system (Linux, macOS, Unix)
- Write permissions for creating output directories

## Common Issues

**"Wrong number of arguments"**: Provide exactly one course ID
```bash
./calc_stat.sh 123456  # Correct
```

**"Course not found"**: Ensure the grade file exists
```bash
ls -l 123456.txt  # Check file existence
```

**Compilation errors**: Use correct C standard
```bash
gcc -std=c99 -o program.exe program.c
```

**Permission denied**: Make script executable
```bash
chmod +x calc_stat.sh
```

## Program Architecture

```
calc_stat.sh (orchestrator)
    ├── hist.exe        → Generate histogram
    ├── mean.exe        → Calculate mean
    ├── median.exe      → Calculate median
    ├── min.exe         → Find minimum
    ├── max.exe         → Find maximum
    └── awk             → Process and format results
```

## Notes

- All executables use `.exe` extension for consistency
- The script creates/overwrites `<course_id>_stat/` directory
- Temporary `<course_id>_new/` directory is automatically cleaned up
- Grade values outside 0-100 range will cause errors in histogram generation
- The pass threshold is hardcoded to 55 (can be modified in script)
