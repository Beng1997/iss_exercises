#!/bin/bash

# Check if the script was called with exactly one argument
if [ "$#" -ne 1 ]; then
    echo "Wrong number of arguments" >&2
    exit 1
fi

# Check if the argument is in the format "xxxxxx"
if [[ ! "$1" =~ [0-9]{6} ]]; then
    echo "Argument is not in xxxxxx format" >&2
    exit 1
fi

# Check if a file with the given name exists in the current directory
if [ ! -f "$1".txt ]; then
    echo "Course not found" >&2
    exit 1
fi

# Remove the existing stat directory if it exists
if [ -d "$1"_stat ]; then
    rm -rf "$1"_stat
fi

# Create the new stat directory
mkdir -p "$1"_stat
mkdir -p "$1"_new
# Run the hist.exe executable on the input file and write the output to
# histogram.txt
./hist.exe "$1".txt > "$1"_stat/histogram.txt

./hist.exe "$1".txt -n_bins 20 > "$1"_new/histogram_2.txt 

# Extract the bin count for the bin corresponding to grades -100 and divide it
# by the total number of students
total=$(awk '{s+=$2} END {print s}' "$1"_new/histogram_2.txt )
pass=$(awk -F '\t' '$1 >= 55 {s+=$2} END {print s}' "$1"_new/histogram_2.txt )
pass_percent=$(awk "BEGIN {printf \"%d\", $pass/$total*100}")



# Run the statistics executables on the input file
# and write the output, including the fail percentage, to statistics.txt
mean=$(./mean.exe "$1".txt)
min=$(./min.exe "$1".txt)
max=$(./max.exe "$1".txt)
median=$(./median.exe "$1".txt)

rm -rf "$1"_new

echo -e "$mean\t$median\t$min\t$max\t$pass_percent%">> "$1"_stat/statistics.txt


# Exit with a success code of 0
exit 0
