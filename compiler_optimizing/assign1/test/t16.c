#include <stdio.h>
int main ( int argc, char *argv[] ) {
	int k;
	k = argc + 2;
	while(k > 0){
		switch(k) {
		case 0: {
			if(k %2 == 0) { 
				printf("k \n");
			} else {
				printf("D");
				printf("D\n");
			}
			k -= 1;
			break;
			}
		
		case 1:  {
			printf("CASE 1\n");
			break;
			}
	
		case 2:  {
			printf("saCASE 2\n");
			break;
			}
		case 4:  {
			printf("sasdfsaaCASE 2\n");
			break;
			}

		default:  {
			printf("CASE defaufdsalt\n");
			break;
			}
		}
	}
	return argc + 2;
}