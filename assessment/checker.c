#include <stdio.h>
#include <stdlib.h>


void bin(unsigned n,FILE *fp)
{
    unsigned char i;
    for (i = 1 << 8 - 1; i > 0; i = i / 2)
        (n & i)? fprintf(fp,"1"): fprintf(fp,"0");
}

int main(int argc, char const *argv[])
{
	FILE *fp1 = NULL,*fp2 = NULL;
	unsigned char value;
	if(!(fp1 = fopen("U_byte_data","r")))
	{
		printf("U_byte_data not opened \n");
	}
	if(!(fp2 = fopen("checker","w")))
	{
		printf("checker not opened \n");
	}
	value = fgetc(fp1);
	while(!feof(fp1))
	{
		bin(value,fp2);
		value = fgetc(fp1);
		printf("%d\n",value);
	}
	fclose(fp1);
	fclose(fp2);

	return 0;
}