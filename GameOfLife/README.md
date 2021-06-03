#Purpose

This program creates a structure named "Universe" which
consists of a 2d array that houses cells defined eiter 
"alive" or "dead" or "true" or "false". Depending on user input, the
program will print out the generation of the universe after n generations
depending on the main conditons.



#Build

Note:
	Incorperates extern file "utilities.h" which
	houses external variables moves and compares.
	
Important Commands:

$make all:
	Executes neccesary makefile commands to create executable 
	
$make format:
	clang formats the main c files.
	
$make clean:
	cleans out all binary and output files

	
	
#Run

-Format the main  c files using make format

-Execute the command $make all to construct
 the life binary executable

-Execute the command $./life 
 to recieve instructions how to run the program
 
 	Commands:
 		-s tells the program that ncurses is silenced 
 		-t deterimnes whether the universe is toroidal
 		-n determines number of generations
 		-i for the input file
 		-o for the output file (default is stdout)
 		

 		

