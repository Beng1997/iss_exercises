#
# Definitions
#
CC = gcc
CFLAGS = -g -Wall -std=c99
CCLINK = $(CC)
OBJS = main.o stack.o student.o
EXEC = prog.exe
RM = rm -f

#
# Dependencies
#

$(EXEC): $(OBJS)
	$(CCLINK) $(CFLAGS) -o $(EXEC) $(OBJS)

main.o: main.c stack.h student.h
	$(CC) $(CFLAGS) -c main.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

student.o: student.c student.h stack.h
	$(CC) $(CFLAGS) -c student.c


# Clean
#
clean:
	$(RM) $(EXEC) $(OBJS) 
