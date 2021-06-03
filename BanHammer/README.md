#Purpose

This program first reads in a list of words from 2 files,
newspeak.txt and oldspeak.txt. These words are either classified
as badspeak or oldspeak with newspeak translations. The words are stored
in a bloom filter and a hashtable. The bloom filter will return a guaurantee
of the word not being in the hashtable or that a word may be in the hashtable.
The program displays the correct usage of the words inputted from stdin and a
paragraph explaining the monstrous usage of the words. nodes,linkedlists,hashtables,
bitvectors, and boomfilter ADT's are all implemented.


#Build

Important Commands:
$make all:
	Executes neccesary makefile commands to create executable 
	
$make format:
	clang formats all the .c and .h files to fit clang style.
	
$make clean:
	cleans out all binary and .o files


	
#Run

-Run the executable with 1 or more input files piped to stdin.

-Commands
	-m : specifies that the hashtable and all subsequent ADT's will utilize the 
		 move to front characteristic. Moves the node last searched for to the front
		 of the linkedlist
		 
	-h: specifies the size of the hashtable
	
	-f: specifies the size of the bloomfilter

#Erros

scan-build runs clean
 		

