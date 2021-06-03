#Purpose

This program implements 4 different sorting algorithms
and tracks their comparisons, moves, and array size. It
makes use of a stack ADT as a substitution for the conventional
method of recursion for a quiksort. It also makes use of sets
to record command line arguments.


#Build

Note:
	Incorperates the files utilities.h which houses
	2 variables moves and compares which store algorithm
	move and compare count.

Important Commands:
$make all:
	Executes neccesary makefile commands to create executable 
	
$make format:
	clang formats all the .c and .h files to fit clang style.
	
$make clean:
	cleans out all binary and output files

	
	
#Run

-Format the main c files using make format

-Execute the command $make all or $make to construct
 the neccesary binary executable "sorting"

-Execute the command $./sorting 
 to recieve instructions how to run the program
 
 	Commands:
 		 -a : Employs all sorting algorithms.
 		 -b : Enables Bubble Sort.
		 -s : Enables Shell Sort.
		 -q : Enables Quicksort.
 		 -h : Enables Heapsort.
 		 -r seed : Set the random seed to seed. 
 		 -n size : Set the array size to size. 
		 -p elements : Print out elements number of elements from the array.
 		

