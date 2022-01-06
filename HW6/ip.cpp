#include "ip.h"
#include "string.h"

enum { MAX_MASK=32, MAX_RANGE=255, LST_BYTE=24};
#define SLASH "/"
#define DOT "."
#define END "\0"
#define FULL_NUM 0xFFFFFFFF


IP::IP(String pattern): Field(pattern) {}


/**
 * @brief sets the range of values this port can get
 * @param val: String with range of values
 * @return true if val is valid, false else
 */
bool IP::set_value(String val) {

	if(val.equals(END)){ //If Empty String.
		return false;
	}
	String *slash_split;
	String *dot_split;
	size_t num_after_slash;//Number of strings after splitting by '\'
	size_t num_after_dot;//Number of strings after splitting by '.'
	unsigned int mask_num = 0;
	unsigned int mask = 0;
	unsigned int ip_hex = 0;	//We will turn IP address to hexa.
	unsigned int segment = 0;  //We will use it to create IP num
	val.split(SLASH, &slash_split, &num_after_slash);
	if(sizeof(short) != num_after_slash){
		//If number of '/' was differen than 1
		delete[] slash_split;
		return false;	//(Invalid)
	}
	/*Get mask number*/
	mask_num = slash_split[1].trim().to_integer();
	if(mask_num > MAX_MASK){
		delete[] slash_split;
		return false;	//(Invalid)
	}

	/*Split IP according to '.' sign without mask*/
	slash_split[0].split(DOT, &dot_split, &num_after_dot);
	if(sizeof(int) != num_after_dot){
		//If number of '.' was differen than 3
		delete[] slash_split;
		delete[] dot_split;
		return false;	//(Invalid)
	}

	for(size_t i=0; i < sizeof(int); i++){
		segment = dot_split[i].trim().to_integer();
		if (segment > MAX_RANGE){
			delete[] slash_split;
			delete[] dot_split;
			return false;	//(Invalid)
		}
		ip_hex |= (segment << (LST_BYTE - (sizeof(double) * i)));
	}

	mask = FULL_NUM >> (mask_num);

	if(MAX_MASK == mask_num){
		mask = 0;
	}
	//Mask num can not be zero and 32 at the same time
	if(0 == mask_num){
		mask = FULL_NUM;
	}

	this->range[1] = ip_hex | mask;
	this->range[0] = ip_hex & (~mask);

	delete[] dot_split;
	delete[] slash_split;
	return true;

}


/**
 * @brief checks if val matches the range
 * @param val: String with a port number
 * @return true if matches, false else
 */
bool IP::match_value(String val) const{
	if(val.equals(END)){ //If Empty String.
		return false;
	}
	String *dot_split;
	size_t num_after_dot;//Number of strings after splitting by '.'
	unsigned int ip_hex = 0;	//We will turn IP address to hexa.
	unsigned int segment = 0;  //We will use it to create IP num

	/*Split IP according to '.' sign without mask*/
	val.split(DOT, &dot_split, &num_after_dot);
	if(sizeof(int) != num_after_dot){
		//If number of '.' was differen than 3
		delete[] dot_split;
		return false;	//(Invalid)
	}

	for(size_t i=0; i < sizeof(int); i++){
		segment = dot_split[i].trim().to_integer();
		if (segment > MAX_RANGE){
			delete[] dot_split;
			return false;	//(Invalid)
		}
		ip_hex |= (segment << (LST_BYTE - (sizeof(double) * i)));
	}

	delete[] dot_split;
	if(range[0] <= ip_hex && ip_hex<= range[1]){
		return true;
	}
	return false;
}
