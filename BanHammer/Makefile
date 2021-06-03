CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic


all: banhammer

banhammer: banhammer.o parser.o ll.o node.o hash.o bv.o bf.o speck.o
	$(CC) -o $@ $^

format:
	clang-format -i -style=file *.c *.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o banhammer
