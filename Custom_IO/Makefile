CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic


all: encode decode
	

encode: encode.o io.o word.o trie.o
	$(CC) -o $@ $^
	
decode: decode.o io.o word.o trie.o
	$(CC) -o $@ $^

format:
	clang-format -i -style=file *.c *.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o encode decode
