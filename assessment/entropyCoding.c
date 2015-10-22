#include <stdio.h>
#include <stdlib.h>

#define DCtableSize 12
#define ACtableSize 102

#define dataSize 64

unsigned char mask = 128;

int previousDC = 0;
char firstFlag = 1;

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
	value = abs(value);
	//printf("bitCount value: %d\n",value);
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
	int holder = number;
	// printf("holder:%d \n",holder);
	//char *bitpattern;
	number = abs(number);
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
	// for (i = 0; i < j; ++i)
	// {
	// 	printf("%d\n",code[j]);
	// }
	// printf("\n");
	// printf("holder:%d \n",holder);
	if(holder < 0)
	{
		for (i = 0; i < j; ++i)
		{
			if(code[i] == 1)
				code[i] = 0;
			else 
				code[i] = 1;

		}
	}
	// for (i = 0; i < j; ++i)
	// {
	// 	printf("%d\n",code[j]);
	// }
	// printf("\n");
}

void writeTofile(FILE *fp,char *bitpattern,int sizeOfBitPattern, int *code, int sizeOfCode)
{
	int i;
	for (i = 0; i < sizeOfBitPattern; ++i)
	{
		// printf("%c",bitpattern[i]);
		fprintf(fp,"%c",bitpattern[i]);
	}
	// fprintf(fp,"%s",bitpattern);	
	for (i = 0; i < sizeOfCode; ++i)
	{
		// printf("%d",code[i]);
		fprintf(fp,"%d",code[i]);
	}
	//fprintf(fp,"\n");	
	// printf("\n");	

}



void dpcm(char DC, struct HuffmanDCTable *huffmanDCTable, FILE *encodedData)
{
	int *code;
	int size;
	int j;
	//char *bitpattern;
	if(firstFlag == 1)
	{
		previousDC = DC;
		firstFlag = 0;
	}
	else
		DC -= previousDC;
	size = bitCount(DC);
	// printf("size:%d\n",size);
	code = (int *) malloc(size*sizeof(int));
	getBinary(DC,code,size);
	printf("\nNumber:%d Size:%d Binary:",DC,size);
	binaryDisplay(code,size);
	printf("\n");
	for (j = 0; j < DCtableSize; ++j)
	{
		if(huffmanDCTable[j].size == size)
		{
			// bitpattern = (char *) malloc(huffmanDCTable[j].numberOfBits * sizeof(char));
			// bitpattern = huffmanDCTable[j].bitpattern;
			writeTofile(encodedData,huffmanDCTable[j].bitpattern,huffmanDCTable[j].numberOfBits,code,size);
			break;
		}
	}
	free(code);	

}

void rlcEncode(int count,int data,struct HuffmanACTable *huffmanACTable,FILE *encodedData)
{
	int i;
	//printf("data:%d\n",data);
	int size = bitCount(data);
	// if(size == 1)
	// 	size++;

	int *code;
	code = (int *) malloc(size*sizeof(int));
	getBinary(data,code,size);	
	for (i = 0; i < ACtableSize; ++i)
	{
		//printf("data:%d size:%d count:%d Hsize:%d Hrun:%d code: %s\n",data,size,count,huffmanACTable[i].size,huffmanACTable[i].run,huffmanACTable[i].bitpattern);
		if(huffmanACTable[i].size == size && huffmanACTable[i].run == count)
		{
			//printf("numberOfBits:%d\n",huffmanACTable[i].numberOfBits);
			//printf("rle code: %s\n",huffmanACTable[i].bitpattern);
			// for (j = 0; i < huffmanACTable[i].numberOfBits; ++j)
			// {
			// 	/* code */
			// }
			//rleBitpattern = huffmanACTable[i].bitpattern;
			writeTofile(encodedData,huffmanACTable[i].bitpattern,huffmanACTable[i].numberOfBits,code,size);	

			break;
		}
	}	
}

void rlc(char *dataVector,struct HuffmanACTable *huffmanACTable,FILE *encodedData)
{

	int i,count = 0;
	FILE *fp = NULL;
	if(!(fp = fopen("rle","a+")))
	{
		printf("rle not opened\n");
		return;
	}	
	//printf("\n");
	for (i = 1; i < dataSize; ++i)
	{
		printf("-->dataVector[%d]: %d\n",i,dataVector[i]);
		if(dataVector[i] == 0)
		{
			while(dataVector[i] == 0)
			{
				printf("dataVector[%d]: %d\n",i,dataVector[i]);
				i++;
				if(i > dataSize-1)
					break;
				count++;
			}
			if(i > dataSize-1)
			{
				fprintf(fp,"%d %d\n",count,0);
				fprintf(encodedData,"1010");
				//rlcEncode(count,0,huffmanACTable,encodedData);
			}
			else
			{
				fprintf(fp,"%d %d\n",count,dataVector[i]);
				rlcEncode(count,dataVector[i],huffmanACTable,encodedData);
			}
			count = 0;
		}
		else
		{
			fprintf(fp,"%d %d\n",count,dataVector[i]);
			rlcEncode(count,dataVector[i],huffmanACTable,encodedData);
		}
	}
	fclose(fp);
}

void entropyCoding(char *dataVector,struct HuffmanDCTable *huffmanDCTable,struct HuffmanACTable *huffmanACTable,FILE *encodedData)
{

	dpcm(dataVector[0],huffmanDCTable,encodedData);
	rlc(dataVector,huffmanACTable,encodedData);
}


// Check for error
void bitPacking(char * fileNameSource, char * fileNameTarget)
{
	char value;
	unsigned char byte = 0;
	unsigned char runTimeMask = mask;
	FILE *fp = NULL,*target = NULL;
	if(!(fp = fopen(fileNameSource,"r")))
	{
		printf("%s not open\n",fileNameSource);
		return ;
	}
	if(!(target = fopen(fileNameTarget,"w")))
	{
		printf("%s not open\n",fileNameTarget);
		return ;
	}
	value = fgetc(fp);
	while(!feof(fp))
	{
		//printf("%c",value);
		if(value == '1')
		{
			//printf("Value:%c\n",value);
			byte |= runTimeMask;
		}
		//printf("%d\n",runTimeMask);
		runTimeMask >>= 1;

		if(runTimeMask == 0)
		{
			//printf("%d\n",byte);
			fprintf(target,"%c",byte);
			byte = 0;
			runTimeMask = mask;
		}
		value = fgetc(fp);
	} 
	if(runTimeMask != 0)
	{
		fprintf(target,"%c",byte);
	}
	fclose(target);
	fclose(fp);


}


int main(int argc, char const *argv[])
{
	
	struct HuffmanDCTable *huffmanDCTable = NULL;
	huffmanDCTable = (struct HuffmanDCTable *) malloc(sizeof(struct HuffmanDCTable) * DCtableSize);
	struct HuffmanACTable *huffmanACTable = NULL;
	huffmanACTable = (struct HuffmanACTable *) malloc(sizeof(struct HuffmanACTable) * ACtableSize);	
	int i;
	char dataVector[dataSize] = {0};
	int temp;

	FILE *fp = NULL;
	FILE *encodedData = NULL;
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
	fclose(fp);
	// for (i = 0; i < ACtableSize; ++i)
	// {
	// 	//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].bitpattern);
	// 	printf("%d %d %d %s\n",huffmanACTable[i].run,huffmanACTable[i].size,huffmanACTable[i].numberOfBits,huffmanACTable[i].bitpattern);
	// }	
	
	if(!(fp = fopen("sampleZigZagOneline","r")))
	{
		printf("zigzag_DCT_U \n");
		return -1;
	}
	if(!(encodedData = fopen("encodedData","w")))
	{
		printf("Error Opening encodedData \n");
		return -1;
	}
	i = 0;
	while((fscanf(fp, "%d ", &temp)) > 0) 
	{       

		dataVector[i] = temp;
		printf("***> dataVector[%d]: %d\n",i,dataVector[i]);
		i++;
		if(i == dataSize)
		{
			i = 0;
			entropyCoding(dataVector,huffmanDCTable,huffmanACTable,encodedData);
			//printf("\n");
		}
	    //printf("%d ",dataVector[i]); // Just to show output.
	    //i++;
	}
	fclose(encodedData);
	bitPacking("encodedData","U_byte_data");
	fclose(fp);
	return 0;

}