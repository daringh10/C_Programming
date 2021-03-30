#include <ncurses.h>
#include <unistd.h> // For usleep ().

#define ROW 0
#define DELAY 50000

int main ( void ) {
	initscr () ; // Initialize the screen .
	curs_set ( FALSE ); // Hide the cursor .
	for ( int col = 0; col < 40; col += 1) {
		clear () ; // Clear the window .
		mvprintw (ROW , col , "o") ; // Displays "o".
		refresh () ; // Refresh the window .
		usleep ( DELAY ); // Sleep for 50000 microseconds .
	}
	endwin () ; // Close the screen .
	return 0;
}
