#include "port.h"
#include "string.h"

enum { MAX = 2 };
#define DASH "-"

/**
 * @brief Constructor with argument
 */
Port::Port(String pattern) : GenericField() {
    this->pattern = pattern;
}

/**
 * @brief Destructor
 */
Port::~Port() {
    // TODO: Implement destructor logic, if necessary
}

/**
 * @brief Sets the range of values this port can have
 * @param val String with the range of values
 * @return True if val is valid, false otherwise
 */
bool Port::set_value(String val) {
    String* substrings;
    size_t size = 0;
    val.split(DASH, &substrings, &size);

    // If there are more/less than two substrings, delete them and return false
    if (size != MAX) {
        if (size != 0) {
            delete[] substrings;
        }
        return false;
    }
    
    range[0] = substrings[0].trim().to_integer();//min
    range[1] = substrings[1].trim().to_integer();//max
    delete[] substrings;

    if (range[0] > range[1]) {
        return false;
    } else {
        return true;
    }
}

/**
 * @brief Checks if val matches the range
 * @param val String with a port number
 * @return True if matches, false otherwise
 */
bool Port::match_value(String val)  {
    String* type;
    size_t size_type = 0;
    val.split("=", &type, &size_type);
    if (size_type != 2) {
        delete[] type;
        return false;
    }
    // If the type is not "port", delete the array and return false
    if (type[0].trim().equals(pattern) == false) { 
        delete[] type;
        return false;
    }

    int port_num = type[1].trim().to_integer();
    delete[] type;
    // If the port number is not in the range, return false
    return (range[0] <= port_num) && (port_num <= range[1]);
}
/**
 * @brief match the value of the rule with the value in the packet
 * @param packet  the value of the field in the packet
 * @return true  if the value is in the range
 * @return false  else
 */
bool Port::match(String packet) {
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
			if (!match_value(out[1].trim())) {
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
