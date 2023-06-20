#ifndef IP_H
#define IP_H

#include "generic-field.h"
#include "String.h"

enum {RANGE =4};

class IP : public GenericField {
private:
  String pattern; /*the name of the field*/
  unsigned int min_ip; /*min valid ip*/
	unsigned int max_ip; /*max valid ip*/  
public:
    IP(String pattern);
    bool match(String packet) ;
    bool match_value(String packet) const ;
    bool set_value(String value) ;
    ~IP() ;
};
#endif // IP_H
