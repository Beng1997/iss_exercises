#ifndef PORT_H
#define PORT_H

#include "generic-field.h"
#include "String.h"

enum {BOUND =2};

class Port : public GenericField {
private:
  String pattern;//the name of the field
  int range[BOUND];  //the range of the field
public:
    Port(String pattern);
    bool match(String packet) ;
    bool set_value(String value) ;
    bool match_value(String packet) ;
    ~Port() ;
};
#endif // PORT_H