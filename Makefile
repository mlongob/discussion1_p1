CC = g++
DEBUG = -Wall -Werror -DDEBUG -g -pg
CFLAGS = -m32 -c
LFLAGS = -m32 -ldl -pthread
EXECUTABLE = problem1

all: $(EXECUTABLE)

$(EXECUTABLE): problem1.cpp
	 $(CC) $(LFLAGS) problem1.cpp libinterrupt.a thread.o -o $(EXECUTABLE)

clean:
	@rm -rf *~ *.o $(EXECUTABLE) dummy

