#include "ip.h"
#include "generic-field.h"
#include "string.h"
#include <iostream>
#include <cstring>

#define MIN_IP 0
#define MAX_IP 255

#define MIN_NUM_OF_BITS 0
#define MAX_NUM_OF_BITS 32

#define HIGHIEST_VALUE 0xFFFFFFFF
/**
 * @brief constructor with argument
 * @param pattern: String with the pattern of the field
 */
IP::IP(String pattern) : GenericField(){
	this->pattern = pattern;//pattern is the name of the field
}
/**
 * @brief destructor
 */
IP :: ~IP() {
	// Destructor implementation
	// Add any destruction code specific to the IP class
}

/**
 * @brief match the value of the field with the value in the packet
 * @param value  the value of the field in the packet
 * @return true  if the value is in the range
 * @return false  else
 */
bool IP::match_value(String value) const { 
	unsigned int ip_num = 0; 
	String *out_dot;
	size_t size_dot;

	value.split(".", &out_dot, &size_dot);/*spliting according to dot*/
	int multiplier=24;
	for(size_t i=0; i<4; i++){
		unsigned int decimal=(unsigned)out_dot[i].trim().to_integer();
		if(decimal < MIN_IP || decimal > MAX_IP) { /*in case not valid byte*/
			delete[] out_dot;
			return false;
		}
		ip_num += (decimal << multiplier);
		multiplier -= 8; 
	}
	delete[] out_dot;

	if(ip_num >= min_ip && ip_num <= max_ip)
		return true;
	return false;
}
	


/**
 * @brief set the value of the field based on the rule
 * @param value   the value of the field in the rule
 * @return true  if the value is valid
 * @return false else
 */
bool IP::set_value(String value) {
	/*for exampel data.value=3.3.64.8/14*/
	String *out_slesh;
	size_t size_slesh;
	value.split("/", &out_slesh, &size_slesh);/*out_slesh[1] contain
	the number after the slesh*/
	int num_of_bits= out_slesh[1].trim().to_integer();/*the number of bits
	we should take from the ip (from left)*/
	if(num_of_bits < MIN_NUM_OF_BITS || num_of_bits > MAX_NUM_OF_BITS) {
		delete[] out_slesh;
		return false; 
	}
	/*num_of_bits should be a sade of ip*/
	String *out_dot;
	size_t size_dot;

	out_slesh[0].split(".", &out_dot, &size_dot);/*spliting according to dot*/
	unsigned int sum=0;
	int multiplier=24;
	for(size_t i=0; i<4; i++){
		int decimal= out_dot[i].trim().to_integer(); /*for exampel decimal=3
		(the first byte in decimal)*/
		if(decimal < MIN_IP || decimal > MAX_IP) { /*in case not valid byte*/
			delete[] out_slesh;
			delete[] out_dot;
			return false;
		}
		sum += (decimal << multiplier); //for exampel sum=3*2^24
		multiplier -= 8; //for exampel multiplier=16
	}
	delete[] out_slesh;
	delete[] out_dot;
	
	int num_of_first_bits= MAX_NUM_OF_BITS-num_of_bits;//for exampel 32-14=18
	unsigned int mask;
	if(num_of_first_bits == MAX_NUM_OF_BITS)
		mask = HIGHIEST_VALUE;
	else mask = ((unsigned int) 1 << num_of_first_bits) - 1;

	min_ip = sum & (~mask); /*(~mask) is the complement of mask*/
	max_ip = sum | mask; /*| is or*/
	return true;
}	

bool IP::match(String packet) {
	String *output_fields; /*will contain the 4 words*/
	size_t size_of_fields; /*typecally there are 4 fields in packet*/
	packet.split(",", &output_fields, &size_of_fields); /*first we want to
	get the 4 fields seperately*/
	for(size_t i=0; i< size_of_fields ; i++)
	{
		size_t size_of;
		String *out;
		String word(output_fields[i]); /*pattern = number*/
		word.split("=", &out, &size_of);/*split according to =*/
		if(out[0].trim().equals(pattern)) {
			if (match_value(out[1].trim())) {//if the value is in the range
				delete[] output_fields;
				delete[] out; 
				return true;
			}
				
		}
		delete[] out;
	}
	delete[] output_fields;
	return false;
}
