CC = clang 
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all : gen dec err

format: decoder.c generator.c hamming.c hamming.h bm.c bm.h 
	clang-format -i -style=file decoder.c generator.c hamming.c bm.c bm.h hamming.h

gen: gen.o hamming.o bm.o
	$(CC) -o gen generator.c hamming.c bm.c
	
dec: dec.o hamming.o bm.o
	$(CC) -o dec decoder.c hamming.c bm.c	
	
err: error.o
	$(CC) -o err error.c 
	
error.o: error.c
	$(CC) $(CFLAGS) -c error.c
	
gen.o: generator.c
	$(CC) $(CFLAGS) -c generator.c

dec.o: decoder.c
	$(CC) $(CFLAGS) -c decoder.c
	
hamming.o: hamming.c
	$(CC) $(CFLAGS) -c hamming.c

bm.o: bm.c
	$(CC) $(CFLAGS) -c bm.c
	
clean:
	rm -f gen dec *.o
	



