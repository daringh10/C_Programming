#include <stdio.h>

main(){

	float celsius, fahr;
	int step, lower, upper;
	
	lower = 0;
	upper = 300;
	step = 20;
	
	celsius = lower;
	
	while (celsius <= upper){
		fahr = (celsius * 9.0/5.0) + 32;
		printf("%3.0f %6.1f\n", celsius, fahr);
		celsius = celsius + step;
		
		}
		
	
	

}
