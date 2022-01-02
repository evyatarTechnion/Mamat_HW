#include "string.h"
#include <cstring>
#include "ip.h"

#define END_STRING '\0'
#define SPACE ' '


// function decleration
char* allocate_and_copy(const char* str,int size);


/**
 * @brief Initiates an empty string
 */
 String::String() {
 	data = new char[1];
	data[0] = END_STRING;
	length = 0;
}


/**
 * @brief Initiates string from other string
 */
String::String(const String &str) {
	length = str.length;
	data = allocate_and_copy(str.data,length);
}


/**
 * @brief Initiates a string from char array
 */
String::String(const char *str) {
	if (str == NULL) {
		data = new char[1];
		data[0] = END_STRING;
		length = 0;
	}
	else {
		length = strlen(str);
		data = allocate_and_copy(str,length);
	}
}


/*destractor
*/
String::~String() {
	if (data != NULL) {
		delete[] data;
	}
}


/**
 * @brief Changes this from String
 */
String& String::operator=(const String &rhs){
	if (this == &rhs) {
		return *this;
	}
	delete[] data;
	length = rhs.length;
	data = allocate_and_copy(rhs.data,length);
	return *this;
}


/**
 * @brief Changes this from char array
 */
String& String::operator=(const char *str){
	if (!(strcmp(data,str))) {
		return *this;
	}
	delete[] data;
	length = strlen(str);
	data = allocate_and_copy(str,length);
	return *this;
}
    

/**
 * @brief Returns true iff the contents of this equals to the
 * contents of rhs
 */
bool String::equals(const String &rhs) const{
	if ((length == rhs.length) && !strcmp(data, rhs.data)){
		return true;
	}
	return false;
}


/**
 * @brief Returns true iff the contents of this equals to rhs
 */
bool String::equals(const char *rhs) const{
	if (!(strcmp(data,rhs))) {
		return true;
	}
	return false;
}


/**
 * @brief Splits this to several sub-strings according to delimiters.
 * Allocates memory for output that the user must delete (using delete[]).
 * @note Does not affect this.
 * @note If "output" is set to NULL, do not allocated memory, only
 * compute "size".
 */
void String::split(const char *delimiters,String **output,size_t *size) const {
	if(delimiters == NULL || data == NULL) {
		return;
	}

	int substrings_num = 1;
	int num_delimiters;
	int len = length;
	char cloned_data[len+1];
	strcpy(cloned_data,data);
	num_delimiters = strlen(delimiters);

	for (int i = 0; i < len; i++) {
		for(int i_delimeter = 0; i_delimeter < num_delimiters; i_delimeter++) {
			if (cloned_data[i] == delimiters[i_delimeter]) {
				cloned_data[i] = END_STRING;
				if ( i > 0 && cloned_data[i-1] != END_STRING && i < (len-1) ) {
					substrings_num++;
				}
			}
		}
	}

	*size = substrings_num;
	if (output == NULL) {
		return;
	}

	*output = new String[substrings_num];
	int start = 0;
	int i_substring = 0;
	for (int i = 1; i < len+1; i++) {
		if (cloned_data[i] == END_STRING && cloned_data[i-1] != END_STRING) {
			(*output)[i_substring] = String(&cloned_data[start]);
			i_substring++;
			start = i + 1;
		}
	}
	return;
}


/**
 * @brief Try to convert this to an integer. Returns 0 on error.
 */
int String::to_integer() const{
	return atoi(data);
}


/**
 * @brief Remove any leading or trailing white-spaces.
 * Does not change this.
 */
String String::trim() const{
	int start = 0;
	int end = length - 1;

	while (data[start] == SPACE) {
		start++;
	}

	while (data[end] == SPACE) {
		if (end == start) {
			return String(); // spaces string
		}
		end--;
	}
	end++;
	
	char trim_string[end-start+1];
	strncpy(trim_string,&data[start],end-start);
	trim_string[end-start]=END_STRING;
	//trim_string = allocate_and_copy(&data[start],end-start);
	String new_trim_string = String(trim_string);
	return new_trim_string;
	
}


/**
 * @brief allocates and copy a new string
 * @param *str a pointer to a given string
 * @param size the size of the given string
 * @return a pointer to a string which is a copy of the
 * given string
 */
 char* allocate_and_copy(const char* str,int size){
	if(size <= 0){
		return NULL;
	}
	else return strcpy(new char[size+1],str);
}
