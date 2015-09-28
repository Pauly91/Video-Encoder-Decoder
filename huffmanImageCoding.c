#include <stdio.h>
#include <stdlib.h>

#define tableSize 12
#define dataSize 64
struct HuffmanTable
{
	int size;
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
	size--;
	while(number)
	{
		printf("Number:%d\n",number);
		printf("size:%d\n",size);
		if(number%2 == 0)
		{
			printf("0\n");
			code[size] = 0;
			printf("value:%d\n",code[size]);
		}
		else if(number%2 == 1)
		{
			printf("1\n");
			code[size] = 1;
			printf("value:%d\n",code[size]);

		}

		size--;

		number >>= 1;
		binaryDisplay(code,j);
		printf("\n");
	}
	printf("\n");
	binaryDisplay(code,j);
	printf("\n");
	printf("Code\n");
	for (i = 0; i < j; ++i)
	{
		printf("%d",code[size]);
	}
	printf("\n");
}



int main(int argc, char const *argv[])
{
	
	struct HuffmanTable *huffmanTable = NULL;
	huffmanTable = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable) * tableSize);
	int i,size;
	int *code;
	int data[dataSize] = {16,11,	10,	16,	24,	40,	51,	 61,
					12,12,	14,	19,	26,	58,	60,	 55,
					14,13,	16,	24,	40,	57,	69,	 56,
					14,17,	22,	29,	51,	87,	80,	 62,
					18,22,	37,	56,	68,	109,103, 77,
					24,35,	55,	64,	81,	104,113, 92,
					49,64,	78,	87,	103,121,120,101,
					72,92,	95,	98,	112,100,103, 99};
	FILE *fp = NULL;
	FILE *codeFile = NULL;
	if(!(fp = fopen("huffmanDCTable","r")))
	{
		printf("huffmanDCTable not read \n");
		return -1;
	}
	for (i = 0; i < tableSize; ++i)
	{
		//fscanf(fp,"%d %s",&huffmanTable[i].size,huffmanTable[i].bitpattern);
		fscanf(fp,"%d %d",&huffmanTable[i].size,&size);
		huffmanTable[i].bitpattern = (char *) malloc(sizeof(char) * size);
		fscanf(fp,"%s",huffmanTable[i].bitpattern);
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
		binaryDisplay(code,size);
		printf("\n");
		free(code);
	}

	return 0;

}