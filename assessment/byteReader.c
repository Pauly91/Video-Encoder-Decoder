#include <stdio.h>
#include <stdlib.h>


#define byteSize 8



void byteToBitConverter(char * fileName,char *target)
{
	char byte;
	int i;
	unsigned char mask = 0b10000000;
	unsigned char runMask;
	FILE *fp = NULL, *bitFile = NULL;
	
	if (!(fp = fopen(fileName,"r")))
	{
		printf("%s not open\n",fileName);
		return;
	}
	if (!(bitFile = fopen(target,"w")))
	{
		printf("%s not open\n",target);
		return;
	}
	while(fscanf(fp,"%c",&byte) > 0)
	{
		printf("%c - ", byte);
		runMask = mask;
		for (i = 0; i < byteSize; ++i)
		{
			if(byte & runMask)
			{
				fprintf(bitFile,"1");
			}
			else
			{
				fprintf(bitFile,"0");
			}
			runMask >>= 1;
		}
		printf("\n");
	}
	fprintf(bitFile,"\n");
	fclose(fp);
	fclose(bitFile);
}


int main(int argc, char const *argv[])
{

	byteToBitConverter("V_Byte_data","data_unpacked");
	return 0;
}