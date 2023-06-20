#include "string.h"
#include <cstring>


#define END_STRING '\0'
#define SPACE ' '

/**
 * @brief Initiates an empty string
 */
String::String() {
    this->length = 0;
    this->data = new char[1];
    this->data[0] = '\0'; 
}

/**
 * @brief Initiates string from another string
 */
String::String(const String& s) {
    this->length = s.length;
    this->data = new char[this->length + 1];
    strcpy(this->data, s.data);
}

/**
 * @brief Initiates a string from a char array
 */
String::String(const char* str) {
    this->length = strlen(str);
    this->data = new char[this->length + 1];
    strcpy(this->data, str);
}

String::~String() {
    delete[] this->data;
}

/**
 * @brief Changes this from String
 */
String& String::operator=(const String& s) {
    if (this == &s) {
        return *this;
    }
    delete[] this->data;
    this->length = s.length;
    this->data = new char[this->length + 1];
    strcpy(this->data, s.data);
    return *this;
}

/**
 * @brief Changes this from char array
 */
String& String::operator=(const char* str) {
    delete[] this->data;
    this->length = strlen(str);
    this->data = new char[this->length + 1];
    strcpy(this->data, str);
    return *this;
}

/**
 * @brief Returns true iff the contents of this equals to the
 * contents of rhs
 */
bool String::equals(const String &rhs) const{
    if (this->length != rhs.length) {
        return false;
    }
    return strcmp(this->data, rhs.data) == 0;
}

/**
 * @brief Returns true iff the contents of this equals to rhs
 */
bool String::equals(const char *rhs) const{
    if(this->data == NULL && rhs == NULL) {
        return true;
    }
    if (this->data == NULL || rhs == NULL) {
        return false;
    }
    return strcmp(this->data, rhs) == 0;
}
/**
 * @brief Splits this to several sub-strings according to delimiters.
 * Allocates memory for output that the user must delete (using delete[]).
 * @note Does not affect this.
 * @note If "output" is set to NULL, do not allocated memory, only
 * compute "size".
 */
void String::split(const char *delimiters,String **output,size_t *size) const{
	if(delimiters == NULL || this->data == NULL) {
		return;
	}

	int substrings_num = 1;//Number of substrings
	int num_delimiters;//Number of delimiters
	int len = length;
	char clone[len + 1];
	strcpy(clone,data);
	num_delimiters = strlen(delimiters);

	for (int i = 0; i < len; i++) {//Count the number of substrings
		for(int j = 0; j < num_delimiters; j++) {//Count the number of delimiters
			if (clone[i] == delimiters[j]) {
				clone[i] = END_STRING;//Replace the delimiter with '\0'
				if ( (i > 0) && (clone[i-1] != END_STRING) && (i < (len-1) )) {
					substrings_num++;
				}
			}
		}
	}
	//return the size even if output is NULL
	*size = substrings_num;
	if (output == NULL) {
		return;
	}

	*output = new String[substrings_num];//Allocate memory for output
	int start = 0;
	int i_substring = 0;
	for (int i = 1; i < len+1; i++) {
		//If the current char is '\0' and the previous char is not '\0'
		if (clone[i] == END_STRING && clone[i-1] != END_STRING) {
			//Copy the substring to the output
			(*output)[i_substring] = String(&clone[start]);
			i_substring++;
			start = i + 1;
		}
	}
	return;
}



/**
 * @brief Converts the string to an integer
 */
int String::to_integer() const {
    int num = 0;
    int i = 0;
	//If the string is empty
    if (this->data[0] == '-') {//If the string is negative
        i = 1;
    }
	
    while ((size_t)i < this->length) {
        num = num * 10 + (this->data[i] - '0');//Convert char to int
        i++;
    }

    if (this->data[0] == '-') {
        num *= -1;
    }

    return num;
}

/**
 * @brief Removes leading and trailing whitespace from the string
 */
String String::trim() const{
	int start = 0;
	int end = length - 1;

	while (data[start] == SPACE) {
		start++;
	}

	while (data[end] == SPACE) {
		if (end == start) {
			return String(); // spaces string
		}
		end--;
	}
	end++;
	
	char trim_string[end-start+1];//Allocate memory for the trimmed string
	strncpy(trim_string,&data[start],end-start);//Copy the trimmed string
	trim_string[end-start]=END_STRING;
	//trim_string = allocate_and_copy(&data[start],end-start);
	String new_trim_string = String(trim_string);
	return new_trim_string;
	
}

