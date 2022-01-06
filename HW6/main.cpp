#include "port.h"
#include "ip.h"
#include "input.h"

enum { ERROR=-1, RULE_SUBS=2 };

#define DELIM "="
#define SOURCE_IP "src-ip"
#define SOURCE_PORT "src-port"
#define DEST_IP "dst-ip"
#define DEST_PORT "dst-port"


/**
 * @brief main method of rule initialization, value setting and parsing
 * @param argc the amount of input arguments
 * @param argv a pointer to a string which consists of the input arguments
 * @return ERROR in any kind of error and 0 otherwise
 */
int main(int argc, char **argv) {

	if (check_args(argc, argv)) {
		return ERROR;
	}

	String argument(argv[1]);
	String *substrings;
	size_t size = 0;

	argument.split(DELIM, &substrings, &size);
	/* check if there are more/less than two substrings */
	if ((int)size != RULE_SUBS) {
		delete[] substrings;
		return ERROR;
	}

	String field_type = substrings[0].trim();
	String field_value = substrings[1].trim();

	if (field_type.equals(SOURCE_PORT) || field_type.equals(DEST_PORT)) {

		Port input_port(field_type);
		input_port.set_value(field_value);
		parse_input(input_port);
	}
	else if (field_type.equals(SOURCE_IP) || field_type.equals(DEST_IP)) {

		IP input_ip(field_type);
		input_ip.set_value(field_value);
		parse_input(input_ip);
	}
	else {
		delete[] substrings;
		return ERROR;
	}

	delete[] substrings;
	return 0;
}
