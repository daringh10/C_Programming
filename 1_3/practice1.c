#include <stdio.h>

main(){
	int fahr;
	
	#define UPPER 300
	#define LOWER 0
	#define STEP 20
	
	for (fahr=UPPER; fahr>=LOWER; fahr=fahr-STEP)
		printf("%3d %6.1f\n", fahr, (5.0/9.0) * (fahr-32));
}
