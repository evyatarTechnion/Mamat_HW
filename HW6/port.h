#ifndef PORT_H
#define PORT_H

#include "string.h"
#include "field.h"

enum { BOUNDS=2 };


class Port: public Field {

private:
	int range[BOUNDS];

public:

	/**
	 * @brief constructor with argument
	 * @param pattern
	 */
	Port(String pattern);

	/**
	 * @brief sets the range of values this port can get
	 * @param val: String with range of values
	 * @return true if val is valid, false else
	 */
	virtual bool set_value(String val);

	/**
	 * @brief checks if val matches the range
	 * @param val: String with a port number
	 * @return true if matches, false else
	 */
	virtual bool match_value(String val) const;
};


#endif
