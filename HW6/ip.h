#ifndef IP_H
#define IP_H

#include "string.h"
#include "field.h"


class IP : public Field {

private:
	int low_feasible_ip;
	int high_feasible_ip;
	int mask;
	int given_ip;

	/**
	 * @brief convers a String typed which containes the ip in its compact form
	 * @param ip_to_unite this is the variable which is to be converted
	 * @return int which is the converted form of the ip.
	 */
	int united_ip(String ip_to_unite) const;
	// an explanation is the cpp file

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
