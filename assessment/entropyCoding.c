#include <stdio.h>
#include <stdlib.h>

#define DCtableSize 12
#define ACtableSize 102

#define dataSize 64
struct HuffmanDCTable
{
	int size;
	int numberOfBits;
	char *bitpattern;
}; 
struct HuffmanACTable
{
	int run;
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
	
	struct HuffmanDCTable *huffmanDCTable = NULL;
	huffmanDCTable = (struct HuffmanDCTable *) malloc(sizeof(struct HuffmanDCTable) * DCtableSize);
	struct HuffmanACTable *huffmanACTable = NULL;
	huffmanACTable = (struct HuffmanACTable *) malloc(sizeof(struct HuffmanACTable) * ACtableSize);	
	int i,j,size;
	int *code;
	char *bitpattern = NULL;
	char dataVector[dataSize] = {0};
	int temp;

	FILE *fp = NULL;
	if(!(fp = fopen("huffmanDCTable","r")))
	{
		printf("huffmanDCTable not read \n");
		return -1;
	}
	for (i = 0; i < DCtableSize; ++i)
	{
		//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].bitpattern);
		fscanf(fp,"%d %d",&huffmanDCTable[i].size,&huffmanDCTable[i].numberOfBits);
		huffmanDCTable[i].bitpattern = (char *) malloc(sizeof(char) * huffmanDCTable[i].numberOfBits);
		fscanf(fp,"%s",huffmanDCTable[i].bitpattern);
	}
	if(!(fp = fopen("huffmanACTable","r")))
	{
		printf("huffmanACTable not read \n");
		return -1;
	}
	for (i = 0; i < ACtableSize; ++i)
	{
		//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].bitpattern);
		fscanf(fp,"%d %d %d",&huffmanACTable[i].run,&huffmanACTable[i].size,&huffmanACTable[i].numberOfBits);
		huffmanACTable[i].bitpattern = (char *) malloc(sizeof(char) * huffmanACTable[i].numberOfBits);
		fscanf(fp,"%s",huffmanACTable[i].bitpattern);
	}
	for (i = 0; i < ACtableSize; ++i)
	{
		//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].bitpattern);
		printf("%d %d %d %s\n",huffmanACTable[i].run,huffmanACTable[i].size,huffmanACTable[i].numberOfBits,huffmanACTable[i].bitpattern);
	}	
	// fclose(fp);
	// if(!(fp = fopen("zigzag_DCT_U","r")))
	// {
	// 	printf("zigzag_DCT_U \n");
	// 	return -1;
	// }
	// i = 0;
	// while((fscanf(fp, "%d ", &temp)) > 0) 
	// {       

	// 	dataVector[i] = temp;
	// 	printf("%d ",dataVector[i]);
	// 	i++;
	// 	if(i == dataSize)
	// 	{
	// 		i = 0;
	// 		printf("\n");
	// 	}
	//     //printf("%d ",dataVector[i]); // Just to show output.
	//     //i++;
	// }

	// for (i = 0; i < dataSize; ++i)
	// {
	// 	size = bitCount(data[i]);
	// 	printf("size:%d\n",size);
	// 	code = (int *) malloc(size*sizeof(int));
	// 	getBinary(data[i],code,size);
	// 	printf("\nNumber:%d Size:%d Binary:",data[i],size);
	// 	//binaryDisplay(code,size);
	// 	printf("\n");
	// 	for (j = 0; j < tableSize; ++j)
	// 	{
	// 		if(huffmanDCTable[j].size == size)
	// 		{
	// 			bitpattern = (char *) malloc(huffmanDCTable[j].numberOfBits * sizeof(char));
	// 			bitpattern = huffmanDCTable[j].bitpattern;
	// 			writeTofile(fp,bitpattern,huffmanDCTable[j].numberOfBits,code,size);
	// 			//free(bitpattern);
	// 			break;
	// 		}
	// 	}
		
	// 	free(code);
	// }
	fclose(fp);
	return 0;

}