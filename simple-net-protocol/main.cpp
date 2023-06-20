#include "input.h"
#include "ip.h"
#include "port.h"
#include "generic-field.h"
#include <iostream>

enum {ERROR = -1};

/**
 * @brief main method of rule initialization, value setting and parsing
 * @param argc the amount of input arguments
 * @param argv a pointer to a string which consists of the input arguments
 * @return ERROR in any kind of error and 0 otherwise
 */
int main(int argc, char **argv) {
    if(check_args(argc, argv)){ 
        return ERROR; 
    }

    String argument(argv[1]);//The argument we got from the user
	String *substrings;//Array of strings after splitting by ','
	size_t size = 0;

    argument.split("=", &substrings, &size);//Split by '='

    if(size != 2){// the argument is not in the right format
        delete[] substrings;
        return ERROR;
    }

    String type = substrings[0].trim();//Type of rule
    String value = substrings[1].trim();//Value of rule
    //check if the type is either src-ip, dst-ip
    if((type.equals("src-ip")) || (type.equals("dst-ip"))){
        IP ip(type);
        ip.set_value(value);//Set the value of the IP
        parse_input(ip); 
    }//check if the type is either src-port, dst-port
    else if((type.equals("src-port")) || (type.equals("dst-port"))){
        Port port(type);//Create a port object
        port.set_value(value);//Set the value of the port
        parse_input(port); 
    }
    else{
        delete[] substrings;
        return ERROR;
    }
    delete[] substrings;
    return 0;
}

    
   
