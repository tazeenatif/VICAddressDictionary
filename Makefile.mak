# Fill this makefile with the makefile format instructions to 
#   build your program.

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -g

OBJS = main.o dictionary.o csv.o bitstring.o

all: dict1

dict1: $(OBJS)
	$(CC) $(CFLAGS) -o dict1 $(OBJS)

main.o: main.c dictionary.h csv.h bitstring.h record.h
dictionary.o: dictionary.c dictionary.h record.h bitstring.h
csv.o: csv.c csv.h record.h
bitstring.o: bitstring.c bitstring.h

clean:
	rm -f *.o dict1
