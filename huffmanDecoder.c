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

// int getDecimal(int *data, int size)
// {
// 	int i,j;
// 	int decimal = 0
// 	for(i = size - 1; i > 0; --i)
// 	{
// 		decimal += pow(2,size - 1 - i ) * data[i]
// 	}
// 	printf("%d\n",decimal);
// 	return decimal;
// }

int main(int argc, char const *argv[])
{
	FILE *fp = NULL;
	struct HuffmanTable *huffmanTable = NULL;
	huffmanTable = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable) * tableSize);
	char encodedData;
	int data;
	int decodeData[dataSize];
	int step;
	int i,j,k = 0;
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
	if(!(fp = fopen("codedData","r")))
	{
		printf("File Not Open\n");
		return -1;
	 }
	// do
	// {
	// 	encodedData = fgetc(fp);
	// 	if( encodedData == 0)
	// 	{
	// 		encodedData = fgetc(fp);
	// 		if(encodedData == 0))// 00 size 0
	// 		{
	// 			step = 0
	// 			data[k] = 0;
	// 			k++;
	// 		}
	// 		else if (encodedData == 1)
	// 		{
	// 			encodedData = fgetc(fp);
	// 			if(encodedData == 0)) // 010 size 1
	// 			{
	// 				step = 1
	// 				data = (int *) malloc(sizeof(int) * step);
	// 				for (i = 0; i < step; ++i)
	// 				{
	// 					data[i] = fgetc(fp)
	// 				}
	// 				data[k] = getDecimal ;
	// 				k++;
	// 			}
	// 			else if(encodedData == 1)) // 011 size 2
	// 			{
	// 				step = 2
	// 				data = (int *) malloc(sizeof(int) * step);
	// 				for (i = 0; i < step; ++i)
	// 				{
	// 					data[i] = fgetc(fp)
	// 				}
	// 				data[k] = getDecimal ;
	// 				k++;
	// 			}
	// 		}
	// 	}
	// 	else if( encodedData == 1)
	// 	{
	// 		encodedData = fgetc(fp);
	// 		if(encodedData == 0)
	// 		{
	// 		    encodedData = fgetc(fp);				
	// 			if(encodedData == 0)// 100 size 3
	// 			{
	// 				step = 3
	// 				data = (int *) malloc(sizeof(int) * step);
	// 				for (i = 0; i < step; ++i)
	// 				{
	// 					data[i] = fgetc(fp)
	// 				}
	// 				data[k] = getDecimal ;
	// 				k++;
	// 			}
	// 			encodedData = fgetc(fp);
	// 			if(encodedData == 1)// 101 size 4
	// 			{
	// 				step = 4
	// 				data = (int *) malloc(sizeof(int) * step);
	// 				for (i = 0; i < step; ++i)
	// 				{
	// 					data[i] = fgetc(fp)
	// 				}
	// 				data[k] = getDecimal ;
	// 				k++;
	// 			}		
	// 		}
	// 		else if(encodedData == 1))
	// 		{
	// 		    encodedData = fgetc(fp);				
	// 			if(encodedData == 0)// 110 size 5
	// 			{
	// 				step = 5
	// 				data = (int *) malloc(sizeof(int) * step);
	// 				for (i = 0; i < step; ++i)
	// 				{
	// 					data[i] = fgetc(fp)
	// 				}
	// 				data[k] = getDecimal ;
	// 				k++;
	// 			}
	// 			else if(encodedData == 1)	
	// 			{
	// 				encodedData = fgetc(fp);				
	// 				if(encodedData == 0)// 1110 size 6
	// 				{
	// 					step = 6
	// 					data = (int *) malloc(sizeof(int) * step);
	// 					for (i = 0; i < step; ++i)
	// 					{
	// 						data[i] = fgetc(fp)
	// 					}
	// 					data[k] = getDecimal ;
	// 					k++;
	// 				}
	// 				if(encodedData == 1)
	// 				{
	// 					encodedData = fgetc(fp);	
	// 					if(encodedData == 0)// 11110 size 7
	// 					{
	// 						step = 7
	// 						data = (int *) malloc(sizeof(int) * step);
	// 						for (i = 0; i < step; ++i)
	// 						{
	// 							data[i] = fgetc(fp)
	// 						}
	// 						data[k] = getDecimal ;
	// 						k++;
	// 					}						
	// 				}

	// 			}			
	// 		}	
	// 	}
	// 	printf("%c",encodedData);
	// }while(!feof(fp));
	return 0;
}