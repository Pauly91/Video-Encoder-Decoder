#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int value;
	FILE *fp = NULL;
	if (!(fp = fopen("testdata","r")))
	{
		printf("File not opened\n");
		return -1;
	}
	while(!feof(fp))
	{
		fscanf(fp,"%d ",&value);
		printf("%d ",value);
	}
	return 0;
}