#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TableSize 12
#define ACtableSize 162
#define MaxPixel 64

#define byteSize 8
int count = 0;
int previousDC = 0;

struct HuffmanDCTable
{
	int size;
	int codeLength;
	char * code;
};

struct nodes
{
	char bit;
	int size;
	int codeLength;
	int run;
	char *code;
	struct nodes *left,*right;
};

struct HuffmanACTable
{
	int run;
	int size;
	int codeLength;
	char *code;
}; 

struct nodes *DCHead = NULL;
struct nodes *ACHead = NULL;

void createNode(struct nodes *node)
{
	node = (struct nodes *) malloc(sizeof(struct nodes));
}

void printPreOrder(struct nodes *tree)
{
	if(tree)
	{
		printf("bit: %c size: %d codeLength: %d code: %s \n",tree->bit,tree->size,tree->codeLength,tree->code);
		printPreOrder(tree->left);
		printPreOrder(tree->right);
	}
}
// Over creating things
void treeCreatorDC(struct nodes *node, struct HuffmanDCTable *huffmanDCTable, int index,int count)
{

	// //printf("%d %d %d %s\n",count,huffmanDCTable[count].size, huffmanDCTable[count].codeLength, huffmanDCTable[count].code);
	// //printf("index: %d bit: %c \n",index,huffmanDCTable[count].code[index]);
	if(huffmanDCTable[count].code[index] == '0')
	{
		//printf("Left->bit: %c \n",huffmanDCTable[count].code[index]);	
		if(node->left == NULL)
		{
			node->left = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->left;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanDCTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->left;
		//printf("**Left**\nbit: %c bit %c\n",node->bit,huffmanDCTable[count].code[index]);

	}
	else
	{
		//printf("Right->bit: %c \n",huffmanDCTable[count].code[index]);
		if(node->right == NULL)
		{
			node->right = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->right;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanDCTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->right;
		//printf("**Right**\nbit: %c bit %c\n",node->bit,huffmanDCTable[count].code[index]);
	}
	node->size = -1;
	node->codeLength = -1;
	node->code = NULL;
	//printf("index: %d codeLength: %d \n\n\n",index,huffmanDCTable[count].codeLength - 1);
	if(index == huffmanDCTable[count].codeLength - 1)
	{
		node->size = huffmanDCTable[count].size;
		node->codeLength = huffmanDCTable[count].codeLength;
		node->code = (char *) malloc(node->codeLength * sizeof(char));
		strcpy(node->code,huffmanDCTable[count].code);
		node->left = NULL;
		node->right = NULL;
		//printf("\n*****Hit Leaf Node*******\n\n\n");
		if(count == TableSize -1)
			return;
		//printf("DCHead->bit:%c\n",DCHead->bit);
		treeCreatorDC(DCHead,huffmanDCTable,0,++count);
		
	}
	else
	{
		treeCreatorDC(node,huffmanDCTable,++index,count);
	}
	return;

}

void treeCreatorAC(struct nodes *node, struct HuffmanACTable *huffmanACTable, int index,int count)
{

	//printf("%d %d %d %s\n",count,huffmanACTable[count].size, huffmanACTable[count].codeLength, huffmanACTable[count].code);
	//printf("index: %d bit: %c \n",index,huffmanACTable[count].code[index]);
	if(huffmanACTable[count].code[index] == '0')
	{
		//printf("Left->bit: %c \n",huffmanACTable[count].code[index]);	
		if(node->left == NULL)
		{
			node->left = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->left;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanACTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->left;
		//printf("**Left**\nbit: %c bit %c\n",node->bit,huffmanACTable[count].code[index]);

	}
	else
	{
		//printf("Right->bit: %c \n",huffmanACTable[count].code[index]);
		if(node->right == NULL)
		{
			node->right = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->right;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanACTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->right;
		//printf("**Right**\nbit: %c bit %c\n",node->bit,huffmanACTable[count].code[index]);
	}
	node->size = -1;
	node->codeLength = -1;
	node->code = NULL;
	//printf("index: %d codeLength: %d \n\n\n",index,huffmanACTable[count].codeLength - 1);
	if(index == huffmanACTable[count].codeLength - 1)
	{
		node->size = huffmanACTable[count].size;
		node->codeLength = huffmanACTable[count].codeLength;
		node->run = huffmanACTable[count].run;
		node->code = (char *) malloc(node->codeLength * sizeof(char));
		strcpy(node->code,huffmanACTable[count].code);
		node->left = NULL;
		node->right = NULL;
		//printf("\n*****Hit Leaf Node*******\n\n\n");
		if(count == ACtableSize -1)
			return;
		//printf("ACHead->bit:%c\n",ACHead->bit);
		treeCreatorAC(ACHead,huffmanACTable,0,++count);
		
	}
	else
	{
		treeCreatorAC(node,huffmanACTable,++index,count);
	}
	return;

}


int numberFinder(char * code,int size)
{
	int i;
	int mask = 1;
	int number = 0;
	//printf("size: %d\n",size);
	if(code[0] == '0')
	{
		for (i = size-1; i >= 0; --i)
		{
			//printf("code[%d]: %c\n",i,code[i]);
			if(code[i] == '0')
			{
				number |= mask;
			}
			mask <<= 1;
		}
			return -1*number;
	}
	else
	{
		for (i = size-1; i >= 0; --i)
		{
			//printf("code[%d]: %c\n",i,code[i]);
			if(code[i] == '1')
			{
				number |= mask;
			}
			mask <<= 1;
		}
		return number;	
	}




}

struct nodes * findCode(struct nodes *node, char bit)
{
	if(bit == '0')
		return node->left;
	else
		return node->right;
}



int compare_function(const void *a,const void *b) 
{
	struct HuffmanACTable *x = (struct HuffmanACTable *) a;
	struct HuffmanACTable *y = (struct HuffmanACTable *) b;
	return strcmp(x->code,y->code);
	// if (x->probabilty < y->probabilty) 
	// 	return 1;
	// else if (x->probabilty > y->probabilty) 
	// 	return -1; 
	// return 0;
}


void byteToBitConverter(char * fileName,char *target)
{
	char byte;
	int i;
	int count = 0;
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
		runMask = mask;
		for (i = 0; i < byteSize; ++i)
		{
			
			if(byte & runMask)
			{
				fprintf(bitFile,"1 ");
			}
			else
			{
				fprintf(bitFile,"0 ");
			}
			runMask >>= 1;
			count++;
			if(count > MaxPixel -1)
			{
				count = 0;
				break;
			}
		}
	}
	fprintf(bitFile,"\n");
	fclose(fp);
	fclose(bitFile);
}

int main(int argc, char const *argv[])
{
	FILE *fp = NULL,*decode = NULL;
	
	char bit;
	char *patternHolder = NULL;
	char endCode[4] = "0101";

	int i,j;
	int dcReadFlag = 1;
	int endOfFrameCount = 0;
	int dcFlag = 0;
	int DCValue,ACValue,previousDC;
	int size;
	int endOfBlock = 0;
	int numberOfPixel = 0;
	int numberOfZero;
	int temmp;

	struct HuffmanDCTable *huffmanDCTable;
	struct HuffmanACTable *huffmanACTable = NULL;
	struct nodes *holder = NULL;
	//holder = (struct nodes *) malloc(sizeof(struct nodes));
	huffmanACTable = (struct HuffmanACTable *) malloc(sizeof(struct HuffmanACTable) * ACtableSize);	
	DCHead = (struct nodes *) malloc(sizeof(struct nodes));
	DCHead->left = NULL;
	DCHead->right = NULL;
	DCHead->bit = 'H';

	ACHead = (struct nodes *) malloc(sizeof(struct nodes));
	ACHead->left = NULL;
	ACHead->right = NULL;
	ACHead->bit = 'H';
	byteToBitConverter("U_Byte_data","data_unpacked");
	huffmanDCTable = (struct HuffmanDCTable *) malloc(TableSize * sizeof(struct HuffmanDCTable));
	if (!(fp = fopen("huffmanDCTable","r")))
	{
		printf("huffmanDCTable not opened\n");
		return -1;
	}	
	for (i = 0; i < TableSize; ++i)
	{
		fscanf(fp,"%d %d",&huffmanDCTable[i].size,&huffmanDCTable[i].codeLength);
		huffmanDCTable[i].code = (char *) malloc(huffmanDCTable[i].codeLength * sizeof(char));
		fscanf(fp,"%s",huffmanDCTable[i].code);
		////printf("%d %d %s\n",huffmanDCTable[i].size, huffmanDCTable[i].codeLength, huffmanDCTable[i].code);
	}
	fclose(fp);
	treeCreatorDC(DCHead,huffmanDCTable,0,0);
	//printf("\n**PreOrder**\n");
	//printPreOrder(DCHead);	
	if(!(fp = fopen("newHuffmanACtable","r")))
	{
		printf("huffmanACTable not read \n");
		return -1;
	}
	for (i = 0; i < ACtableSize; ++i)
	{
		//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].code);
		fscanf(fp,"%d %d %d",&huffmanACTable[i].run,&huffmanACTable[i].size,&huffmanACTable[i].codeLength);
		huffmanACTable[i].code = (char *) malloc(sizeof(char) * huffmanACTable[i].codeLength);
		fscanf(fp,"%s",huffmanACTable[i].code);
	}
	fclose(fp);

	qsort(huffmanACTable,ACtableSize,sizeof(struct HuffmanACTable),compare_function);	

	// for (i = 0; i < ACtableSize; ++i)
	// {
	// 	//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].code);
	// 	//printf("%d %d %d %s\n",huffmanACTable[i].run,huffmanACTable[i].size,huffmanACTable[i].codeLength,huffmanACTable[i].code);
	// }

	treeCreatorAC(ACHead,huffmanACTable,0,0);
	//printf("\n**PreOrder**\n");
	//printPreOrder(ACHead);	

	if(!(fp = fopen("data_unpacked","r")))
	{
		printf("data_unpacked not read \n");
		return -1;
	}
	if(!(decode = fopen("decode","w")))
	{
		printf("decode not read \n");
		return -1;
	}
	

	while(endOfFrameCount < 32 * 32) // should be 64
	{

		holder = DCHead;
		numberOfPixel = 0;		
		while(!dcFlag) // reset all init values for the loop
		{
			printf("\n*** New Bit DC ***\n");
			fscanf(fp,"%c ",&bit);
			printf("bit Read:%c \n",bit);
			holder = findCode(holder,bit);
			printf("value: %c\n",holder->bit);
			if(holder->size >= 0)
			{
				size = holder->size;
				printf("size: %d\n",size);
				if(size == 0)
				{
					DCValue = 0;
				}
				else
				{
					patternHolder = (char *) malloc(size * sizeof(char));
					i = 0;
					printf("Data bits:");
					while(i < size )
					{

						fscanf(fp,"%c ",&patternHolder[i]); // 0 has the msb
						printf("%c ",patternHolder[i]);
						i++;
					}
					printf("\n");
					printf("code :%s\n",patternHolder);
					DCValue = numberFinder(patternHolder,size); // use of previous DC value
					printf("DC value Before DPCM: %d\n",DCValue);
				}	
				DCValue += previousDC;
				previousDC = DCValue;
				printf("number: %d\n",DCValue);
				numberOfPixel++;
				fprintf(decode, "%d ",DCValue);
				holder = ACHead;
				while(!endOfBlock)
				{
					
					printf("\n*** New Bit AC ***\n");
					printf("Frame: %d pixel:%d\n",endOfFrameCount,numberOfPixel);
					fscanf(fp,"%c ",&bit);
					printf("bit Read:%c \n",bit);
					holder = findCode(holder,bit);
					printf("%c",holder->bit);
					if(holder->size >= 0)
					{
						size = holder->size;
						if(size == 0)
						{
							// put the zeros in files correspoding to numberOfPixel Count
							for (j = numberOfPixel; j < MaxPixel- 1; ++j)
							{
								fprintf(decode, "%d ",0);	
							}
							printf("\nnumber: %d\n",0);
							fprintf(decode, "%d \n",0);
							endOfFrameCount++;	
							break;
						}
						numberOfZero = holder->run;
						printf("\nsize: %d\n",size);
						patternHolder = (char *) malloc(size * sizeof(char));
						i = 0;
						while(i < size )
						{
							
							fscanf(fp,"%c ",&patternHolder[i]); // 0 has the msb
							printf("%c ",patternHolder[i]);
							i++;
						}
						printf("\n");

						printf("code :%s\n",patternHolder);
						ACValue = numberFinder(patternHolder,size); // get the number of Zeros right
						printf("number: %d\n",ACValue);
						for (j = 0; j < numberOfZero; ++j)
						{
							numberOfPixel++;
							fprintf(decode, "%d ",0);	
						}
						numberOfPixel++;
						fprintf(decode, "%d ",ACValue);	
						holder = ACHead;
					}
				}

				break;
			}
		}
	} 
	return 0;
}



/*

check encoding

*/