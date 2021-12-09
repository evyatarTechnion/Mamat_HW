CC		= gcc
CFLAGS	= -g -Wall -std=c99
CCLINK	= $(CC)
RM		= libgrades.so *.o

# library linking
libgrades.so: grades.o
	$(CCLINK) -shared grades.o -o libgrades.so -llinked-list -L.

# linking  
grades.o: grades.c grades.h linked-list.h
	$(CC) $(CFLAG) -c -fpic grades.c

# remove .o .so files
clean:
	rm -fr $(RM)
