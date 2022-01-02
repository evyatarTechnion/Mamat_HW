#include "ip.h"
#include "string.h"

enum { MASK_SUBS=2, MAX_MASK=32, MIN_MASK=0, IP_BYTES=4, BYTE=8, ERROR=-1 };
#define SLASH "/"
#define DOT "."


IP::IP(String pattern): Field(pattern) {}


/**
 * @brief sets the range of values this port can get
 * @param val: String with range of values
 * @return true if val is valid, false else
 */
bool IP::set_value(String val) {

	String *substrings;
	size_t size = 0;

	val.split(SLASH, &substrings, &size);

	/* check if there are more/less than two substrings */
	if(size != MASK_SUBS) {
		delete[] substrings;
		return false;
	}

	int mask = substrings[1].trim().to_integer();

	/* check if the mask is out of size range */
	if (mask > MAX_MASK || mask < MIN_MASK) {
		delete[] substrings;
		return false;
	}

	int bin_mask = (1 << ((IP_BYTES*BYTE) - mask)) -1;

	int unmasked_ip = combine_ip(substrings[0].trim());

	/* check if the unmasked ip is valid */
	if (unmasked_ip == ERROR) {
		delete[] substrings;
		return false;
	}

	range[0] = unmasked_ip & (~bin_mask);
	range[1] = unmasked_ip | bin_mask;
	delete[] substrings;
	return true;

}


/**
 * @brief checks if val matches the range
 * @param val: String with a port number
 * @return true if matches, false else
 */
bool IP::match_value(String val) const {

	int ip_num = combine_ip(val.trim());

	/* check if the ip is valid */
	if(ip_num == ERROR) {
		return false;
	}

	if (range[0] <= ip_num && ip_num <= range[0]) {
		return true;
	}
	return false;
}


/**
 * @brief combine string-form ip into an integer for internal use
 * @param divided_ip the ip we want to combine
 * @return the integer ip
 */
int IP::combine_ip(String divided_ip) const {
	String *substrings;
	size_t size = 0;

	divided_ip.split(DOT, &substrings, &size);

	/* check if there are more/less than four substrings */
	if (size != IP_BYTES){
		delete[] substrings;
		return ERROR;
	}

	int combined_ip = 0;
	for(int i = 0; i < IP_BYTES; i++) {
		combined_ip += ((substrings[i].trim().to_integer()) << (BYTE * i));
	}

	delete[] substrings;
	return combined_ip;
}
