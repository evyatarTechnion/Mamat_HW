#include "ip.h"

enum { NUM_OF_SUBS=2, MAX_MASK=32, MIN_MASK=0 };
#define IP_SEGMENTS 4
#define SLASH "/"
#define IP_ERROR (-1)
#define DOT_SIGN "."
#define IP_SEG_SIZE 8
#define FULL_MASK 0xFFFFFFFF


IP::IP(String pattern): Field(pattern) {}


/**
 * @brief concatenates and shifts a string into an integer
 * @param ip_to_unite a String which is being assessed into an integer
 * @return ERROR is all cases of an error, else the assessted
 * @note a method which has not been introducted in the questionnaire
 */
int IP::united_ip(String ip_to_unite)const{
	String *substrings;
	size_t size=0;

	ip_to_unite.split(DOT_SIGN,&substrings,&size);

	if (size != IP_SEGMENTS){
		delete[] substrings;
		return IP_ERROR;
	}

	int ip_to_return=0;
	int i;

	for(i=0; i<IP_SEGMENTS;i++){
		ip_to_return+=(substrings[i].trim().to_integer())<<(IP_SEG_SIZE*(i));
	}
	delete[] substrings;
	return ip_to_return;
}


/**
 * @brief sets the range of values this port can get
 * @param val: String with range of values
 * @return true if val is valid, false else
 */
bool IP::set_value(String val) {

	String *substrings;
	size_t size = 0;

	val.split(SLASH, &substrings, &size);

	/* if there is more/less than two substrings - delete them */
	if(size != NUM_OF_SUBS) {
		delete[] substrings;
		return false;
	}

	mask = substrings[1].trim().to_integer();

	/* check if the mask is out of size range */
	if (mask > MAX_MASK || mask < MIN_MASK){
		delete[] substrings;
		return false;
	}

	int bitmask = (1 << ((IP_SEG_SIZE*IP_SEGMENTS)-mask)) -1;

	given_ip = united_ip(substrings[0].trim());
	// we'll have to define return error value for latter function, currently 0
	// disputed
	if (given_ip == IP_ERROR){
		delete[] substrings;
		return false;
	}

	low_feasible_ip= given_ip & (~bitmask);
	high_feasible_ip= given_ip | bitmask;
	delete[] substrings;
	return true;

}


/**
 * @brief checks if val matches the range
 * @param val: String with a port number
 * @return true if matches, false else
 */
bool IP::match_value(String val) const{

	int ip_to_check = united_ip(val.trim());
	if(ip_to_check == IP_ERROR){
		return false;
	}
	if (low_feasible_ip <= ip_to_check && ip_to_check <= high_feasible_ip){
		return true;
	}
	return false;
}
