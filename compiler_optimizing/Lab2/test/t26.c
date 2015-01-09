#include <stdlib.h>
#include <stdio.h>

int mbr_test (int a) 
{
	
	switch(a)
	{
		case 1:		printf("*\n");
		case 2:		printf("**\n");
		case 3:		printf("***\n");
		case 4:		printf("****\n");
		default:	printf("*****\n");
	}
	
	switch(a)
	{
		case 1:		printf("*\n");		break;
		case 2:		printf("**\n");		break;
		case 3:		printf("***\n");	break;
		case 4:		printf("****\n");	break;
		default:	printf("*****\n");	break;
	}
	
	return 0;
}

