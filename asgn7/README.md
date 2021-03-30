#Purpose

This program makes use of the Lempel-Ziv compression/decompression
algorithm to compress a file by finding patterns among bytes/characters
and storing them in a k-ary tree, with k representing 256. In a trie, a trie
node is stored with a code at its corresponding symbol's index in the alphabet array, 
and is accessed by traversing the tree until the code is obtainted, with each previous
symbol serving as a prefix the current symbol. This program also implements custom io,
and creates a Word ADT to store characters.


#Build

Important Commands:
$make all:
	Executes neccesary makefile commands to create executable 

$make decode/code:
	Complies the encode/decode binaries individually.
	
$make format:
	clang formats all the .c and .h files to fit clang style.
	
$make clean:
	cleans out all binary and .o files


	
#Run

-Run the executable with 1 or more input files piped to stdin.

-Commands 

	-i : Specifies the input file
		 
	-o: specifies the output file
	
	-v: specifies whether the compression statistics should be printed.

#Erros

When running valgrind on "$make all", there are errors in the
"still reachable" category, but that shouldn't interfere with heap exhaustion

 		

