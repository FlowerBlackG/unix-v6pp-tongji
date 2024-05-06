#include <stdio.h>
#include <stdlib.h>
#include <sys.h>

int main1(int argc, char* argv[])
{
	int i;
	char line[100];
	
	for( i = 1; i < argc; i++)
	{
		printf("%s ", argv[i]);
	}
	
	printf("\n");

	exit(0);

	return 0;
}
