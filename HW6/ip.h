#ifndef IP_H
#define IP_H

#include "string.h"
#include "field.h"

enum { BOUNDS=2 };

class IP : public Field {

private:
	int range[BOUNDS];

	/**
	 * @brief combine string-form ip into an integer for internal use
	 * @param divided_ip the ip we want to combine
	 * @return the integer ip
	 */
	int combine_ip(String divided_ip);

public:

	/**
	 * @brief constructor with argument
	 * @param pattern
	 */
	IP(String pattern);

	/**
	 * @brief sets the range of values this port can get
	 * @param val: String with range of values
	 * @return true if val is valid, false else
	 */
	bool set_value(String val);

	/**
	 * @brief checks if val matches the range
	 * @param val: String with a port number
	 * @return true if matches, false else
	 */
	bool match_value(String val) const;
};

#endif
