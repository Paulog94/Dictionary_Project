CC = gcc
CCFLAGS = -Wall

dictstat:dictstat.o
	$(CC) dictstat.o -o dictstat 

dictstat.o:
	$(CC) $(CCFLAGS) -c dictstat.c dictstat.h

clean:
	rm dictstat.o *.exe *.gch
