CC = clang 
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all : sorting

format: sorting.c  set.c bubble.c quick.c stack.c shell.c heap.c set.h bubble.h quick.h stack.h shell.h heap.h utilities.h gaps.h

	clang-format -i -style=file sorting.c  set.c bubble.c quick.c stack.c shell.c heap.c bubble.h quick.h stack.h shell.h heap.h utilities.h gaps.h

sorting: sorting.o set.o bubble.o quick.o stack.o shell.o heap.o
	$(CC) -o sorting sorting.c set.c bubble.c quick.c stack.c shell.c heap.c

sorting.o:sorting.c
	$(CC) $(CFLAGS) -c sorting.c
set.o:set.c
	$(CC) $(CFLAGS) -c set.c
bubble.o: bubble.c
	$(CC) $(CFLAGS) -c bubble.c
quick.o:quick.c
	$(CC) $(CFLAGS) -c quick.c
stack.o: stack.c 
	$(CC) $(CFLAGS) -c stack.c
shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c
	
clean:
	rm -f sorting *.o
	
