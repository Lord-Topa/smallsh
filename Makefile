CC=gcc --std=gnu99 -g

all: topash

topash: topash.c topash.h 
	$(CC) topash.c topash.h -o topash

clean:
	rm -f *.o topash
