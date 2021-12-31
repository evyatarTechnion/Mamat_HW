#include "port.h"

#define DASH "-"


/**
* @brief constructor with argument
* @param pattern defines if the port is dst or src
*/
Port::Port(String pattern): Field(pattern) {}


/**
 * @brief sets the range of values this port can get
 * @param val: String with range of values
 * @return true if val is valid, false else
 */
bool Port::set_value(String val) {

	String *substrings;
	size_t size = 0;

	val.split(DASH, &substrings, &size);

	/* if there is more/less than two substrings - delete them */
	if (size != BOUNDS) {
		if (size != 0) {
			delete[] substrings;
		}
		return false;
	}

	range[0] = substrings[0].trim().to_integer();
	range[1] = substrings[1].trim().to_integer();

	delete[] substrings;

	if(range[0] > range[1]) {
		return false;
	}
	else {
		return true;
	}
}


/**
 * @brief checks if val matches the range
 * @param val: String with a port number
 * @return true if matches, false else
 */
bool Port::match_value(String val) const {
	int port_num = val.trim().to_integer();
	return ( (range[0] <= port_num) && (port_num <= range[1]) );
}
