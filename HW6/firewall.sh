#!/bin/bash

# all packets in the packets file, removing ' ' and tabs from them 
original_packets=$(cat | tr -s ' ');

while read line; do
	# remove all spaces and tabs from the line
	line=$(echo $line | tr -d ' ' | sed -e 's/#.*//');
	
	# skip empty lines
	if [ ! -z "$line" ]; then

		# for each line update the remained packets to be all
		checked_packets=$original_packets

		# split the string into four. send each command
		 
		for(( field=1; field<5; field++ )) 
		do
		 # get the rule by dividing the line by ','
		rule=$(echo $line | cut -d "," -f$field) 
		checked_packets=$(echo "$checked_packets" | ./firewall.exe "$rule")
		done

		# update the good packet list
		good_packets+=$(echo "$checked_packets")
		# add new line
		good_packets+="\n"

	fi # if empty line

done <$1 

# print good packets but in sorting order, and only one time
echo -e "$good_packets" | grep -vE ^$ | tr -d ' ' | sort | uniq
