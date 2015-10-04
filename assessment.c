#include <stdio.h>
#include <stdlib.h>
#include "bmpReader.h"

int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		printf("\n ./run [file name] \n\n");
		return -1;
	}
	BMPData *image = NULL;
	if((image = readBMPfile(argv[1])) == NULL)
		printf("Error in File 1\n");
			
	return 0;
}