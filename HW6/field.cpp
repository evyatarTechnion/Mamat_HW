#include <iostream>
#include <cstring>
#include "field.h"

#define DELIMETERS ",="
#define TWO 2


/* copy constructor with string input
 */
Field::Field(String pattern){
	this->pattern = pattern;
}


/* destructor - will destruct String pattern
 */
Field::~Field() {
}


/*
 * set_value and match_value are pure virtual methods
 * the implementation will be in the inheritors
 */	


/**
 * @brief This function will go over all fields in packet, and compare using
 * @ 	  match_value the values in the packet only for the field  in packet 
 * @ 	  that is the same is this.
 * @param packet String object contains packet values.
 * @return True if value matched, false if not.
 */
bool Field::match(String packet){

	String *substr;
	size_t size = 0;
	packet.split(DELIMETERS,&substr,&size);

	if (size == 0) { //empty packet
		return false;
	}

	for (int i=0; i<(int)size; i+=TWO){
        if(pattern.equals(substr[i].trim()) && match_value(substr[i+1].trim())){
			delete[] substr;
			return true;

		}
	}
	delete[] substr;
	return false;
}