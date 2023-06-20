#!/bin/bash

link="https://www.ynetnews.com/category/3082"

data=$(wget --no-check-certificate -O - "$link" 2>/dev/null)
# Extract all unique article URLs using a regular expression
articles=$(echo "$data" | grep -oP "https://www.ynetnews.com/article/[a-zA-Z0-9]+" |sort| uniq)

# Convert the string of articles to an array
array_of_article=($(echo "${articles[@]}"))

# Print the number of articles found to the file
echo "${#array_of_article[@]}" >> results.csv

for ((i = 0; i < ${#array_of_article[@]}; i++)); do
    x="0 0 0 0"
    num_of_names=($x)
    # Initialize a counter to count the number of names that do not appear 
    counter=0
    names="Netanyahu Gantz Bennett Perez"

    array_of_names=($(echo "${names[@]}"))

    article_text=$(wget --no-check-certificate -O - "${array_of_article[$i]}" 2>/dev/null \
                    | sed -n '/<article/,/<\/article>/p' \
                    | sed 's/<[^>]*>//g')
                    
    for ((j = 0; j < ${#array_of_names[@]}; j++)); do
        num_of_names[$j]=$(wget --no-check-certificate -O - \
        "${array_of_article[$i]}" 2>/dev/null \
        | grep -oPi  ${array_of_names[$j]} | wc -l)
        # If the name does not appear in the article, increment the counter
        if (( ${num_of_names[$j]}==0 )); then
            (( counter++ ))
        fi
    done 
    # If all names do not appear in the article, 
    # output the article URL with a '-' 
    if (( $counter==${#array_of_names[@]} )); then
        echo "${array_of_article[$i]},-" >> results.csv
    else
    # else, output the article URL followed by the 
    # number of times each name appears in the article
        echo "${array_of_article[$i]},\
        Netanyahu,${num_of_names[0]},\
        Gantz,${num_of_names[1]},\
        Bennett,${num_of_names[2]},\
        Perez,${num_of_names[3]}" >> results.csv  
    fi 
done
