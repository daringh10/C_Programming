#include <stdio.h>

#define IN 1
#define OUT 0


main(){
	int c,blank,tab,newline;
	c = blank = tab = newline = 0;
	
	
	while((c = getchar()) != EOF){
		if (c == ' '){
			++blank;
		}
		if (c == '\n'){
			++newline;
		}
		if (c == '\t'){
			++tab;
		}
	}
	printf("%d %d %d\n", blank, tab, newline);
}
