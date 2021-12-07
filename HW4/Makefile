CC		= gcc
CFLAGS	= -g -Wall -std=c99
CCLINK	= $(CC)
OBJS	= grades.o
SOBJ	= libgrades.so
RM		= libgrades.so *.o

# linking
$(SOBJ): $(OBJS)
	$(CCLINK) -shared $(OBJS) -o $(SOBJ) -llinked-list -L.

# compilation  
grades.o: grades.c grades.h linked-list.h
	$(CC) $(CFLAG) -c -fpic grades.c

# remove .o and .so files
clean:
	rm -fr $(RM)
