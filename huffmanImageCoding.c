#include <stdio.h>
#include <stdlib.h>

#define tableSize 12
#define dataSize 64
struct HuffmanTable
{
	int size;
	int numberOfBits;
	char *bitpattern;
}; 

int bitCount(int value)
{
	int count = 0;
	while(value)
	{
		value >>= 1;
		count++;
	}
	return count;
}
void binaryDisplay(int *code,int size)
{
	int i;
	for (i = 0; i < size; ++i)
	{
		printf("%d",code[i]);
	}
}
void getBinary(int number,int* code, int size)
{
	int j = size,i;
	//char *bitpattern;
	size--;
	while(number)
	{
		if(number%2 == 0)
		{
			code[size] = 0;;
		}
		else if(number%2 == 1)
		{
			code[size] = 1;
		}
		size--;
		number >>= 1;
	}

}

void writeTofile(FILE *fp,char *bitpattern,int sizeOfBitPattern, int *code, int sizeOfCode)
{
	int i;
	for (i = 0; i < sizeOfBitPattern; ++i)
	{
		fprintf(fp,"%c",bitpattern[i]);
	}
	for (i = 0; i < sizeOfCode; ++i)
	{
		fprintf(fp,"%d",code[i]);
	}
	fprintf(fp,"\n");	
}

int main(int argc, char const *argv[])
{
	
	struct HuffmanTable *huffmanTable = NULL;
	huffmanTable = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable) * tableSize);
	int i,j,size;
	int *code;
	char *bitpattern = NULL;
	int data[dataSize] = {  16,11,	10,	16,	24,	40,	51,	 61,
							12,12,	14,	19,	26,	58,	60,	 55,
							14,13,	16,	24,	40,	57,	69,	 56,
							14,17,	22,	29,	51,	87,	80,	 62,
							18,22,	37,	56,	68,	109,103, 77,
							24,35,	55,	64,	81,	104,113, 92,
							49,64,	78,	87,	103,121,120,101,
							72,92,	95,	98,	112,100,103, 99};
	FILE *fp = NULL;
	if(!(fp = fopen("huffmanDCTable","r")))
	{
		printf("huffmanDCTable not read \n");
		return -1;
	}
	for (i = 0; i < tableSize; ++i)
	{
		//fscanf(fp,"%d %s",&huffmanTable[i].size,huffmanTable[i].bitpattern);
		fscanf(fp,"%d %d",&huffmanTable[i].size,&huffmanTable[i].numberOfBits);
		huffmanTable[i].bitpattern = (char *) malloc(sizeof(char) * huffmanTable[i].numberOfBits);
		fscanf(fp,"%s",huffmanTable[i].bitpattern);
	}
	fclose(fp);
	if(!(fp = fopen("codedData","w")))
	{
		printf("codedData not open \n");
		return -1;
	}
	for (i = 0; i < tableSize; ++i)
	{
		printf("%d %s\n",huffmanTable[i].size,huffmanTable[i].bitpattern);
	}
	for (i = 0; i < dataSize; ++i)
	{
		size = bitCount(data[i]);
		printf("size:%d\n",size);
		code = (int *) malloc(size*sizeof(int));
		getBinary(data[i],code,size);
		printf("\nNumber:%d Size:%d Binary:",data[i],size);
		//binaryDisplay(code,size);
		printf("\n");
		for (j = 0; j < tableSize; ++j)
		{
			if(huffmanTable[j].size == size)
			{
				bitpattern = (char *) malloc(huffmanTable[j].numberOfBits * sizeof(char));
				bitpattern = huffmanTable[j].bitpattern;
				writeTofile(fp,bitpattern,huffmanTable[j].numberOfBits,code,size);
				//free(bitpattern);
				break;
			}
		}
		
		free(code);
	}
	fclose(fp);
	return 0;

}