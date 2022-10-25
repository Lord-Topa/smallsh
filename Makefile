CC=gcc --std=gnu99 -g

all: smallsh

smallsh: smallsh.c smallsh.h 
	$(CC) smallsh.c smallsh.h -o smallsh

clean:
	rm -f *.o smallsh
