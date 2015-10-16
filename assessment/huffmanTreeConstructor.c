#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TableSize 12
#define ACtableSize 102

int count = 0;
struct HuffmanDCTable
{
	int size;
	int codeLength;
	char * code;
	struct HuffmanDCTable *left,*right;
};

struct nodes
{
	char bit;
	int size;
	int codeLength;
	char *code;
	struct nodes *left,*right;
};

struct HuffmanACTable
{
	int run;
	int size;
	int numberOfBits;
	char *bitpattern;
}; 

struct nodes *head = NULL;

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
void treeCreator(struct nodes *node, struct HuffmanDCTable *huffmanDCTable, int index,int count)
{

	printf("%d %d %d %s\n",count,huffmanDCTable[count].size, huffmanDCTable[count].codeLength, huffmanDCTable[count].code);
	printf("index: %d bit: %c \n",index,huffmanDCTable[count].code[index]);
	if(huffmanDCTable[count].code[index] == '0')
	{
		printf("Left->bit: %c \n",huffmanDCTable[count].code[index]);	
		if(node->left == NULL)
		{
			node->left = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->left;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanDCTable[count].code[index];
			printf("-->new node\n");
		}
		else
			node = node->left;
		printf("**Left**\nbit: %c bit %c\n",node->bit,huffmanDCTable[count].code[index]);

	}
	else
	{
		printf("Right->bit: %c \n",huffmanDCTable[count].code[index]);
		if(node->right == NULL)
		{
			node->right = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->right;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanDCTable[count].code[index];
			printf("-->new node\n");
		}
		else
			node = node->right;
		printf("**Right**\nbit: %c bit %c\n",node->bit,huffmanDCTable[count].code[index]);
	}
	node->size = -1;
	node->codeLength = -1;
	node->code = NULL;
	printf("index: %d codeLength: %d \n\n\n",index,huffmanDCTable[count].codeLength - 1);
	if(index == huffmanDCTable[count].codeLength - 1)
	{
		node->size = huffmanDCTable[count].size;
		node->codeLength = huffmanDCTable[count].codeLength;
		node->code = (char *) malloc(node->codeLength * sizeof(char));
		strcpy(node->code,huffmanDCTable[count].code);
		node->left = NULL;
		node->right = NULL;
		printf("\n*****Hit Leaf Node*******\n\n\n");
		if(count == TableSize -1)
			return;
		printf("Head->bit:%c\n",head->bit);
		treeCreator(head,huffmanDCTable,0,++count);
		
	}
	else
	{
		treeCreator(node,huffmanDCTable,++index,count);
	}
	return;

}



int numberFinder(char * code,int size)
{
	int i;
	int mask = 1;
	int number = 0;
	printf("size: %d\n",size);
	if(code[0] == '0')
	{
		for (i = size-1; i >= 0; --i)
		{
			printf("code[%d]: %c\n",i,code[i]);
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
			printf("code[%d]: %c\n",i,code[i]);
			if(code[i] == '1')
			{
				number |= mask;
			}
			mask <<= 1;
		}
		return number;	
	}




}

struct nodes * findDcCode(struct nodes *node, char bit)
{
	if(bit == '0')
		return node->left;
	else
		return node->right;
}

int main(int argc, char const *argv[])
{
	FILE *fp = NULL,*decode = NULL;
	
	char bit;
	char *patternHolder = NULL;

	int i;
	int dcReadFlag = 1;
	int endOfFrameCount = 0;
	int dcFlag = 0;
	int DCValue;
	int size;

	struct HuffmanDCTable *huffmanDCTable;
	struct HuffmanACTable *huffmanACTable = NULL;
	struct nodes *holder = NULL;
	//holder = (struct nodes *) malloc(sizeof(struct nodes));
	huffmanACTable = (struct HuffmanACTable *) malloc(sizeof(struct HuffmanACTable) * ACtableSize);	
	head = (struct nodes *) malloc(sizeof(struct nodes));
	head->left = NULL;
	head->right = NULL;
	head->bit = 'H';
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
		//printf("%d %d %s\n",huffmanDCTable[i].size, huffmanDCTable[i].codeLength, huffmanDCTable[i].code);
	}
	fclose(fp);
	treeCreator(head,huffmanDCTable,0,0);
	printf("\n**PreOrder**\n");
	printPreOrder(head);	
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
	if(!(fp = fopen("encodedData","r")))
	{
		printf("encodedData not read \n");
		return -1;
	}
	if(!(decode = fopen("decode","w")))
	{
		printf("decode not read \n");
		return -1;
	}
	holder = head;

	while(endOfFrameCount < 1) // should be 64
	{

		while(!dcFlag) // reset all init values for the loop
		{
			fscanf(fp,"%c ",&bit);
			printf("bit Read:%c \n",bit);
			holder = findDcCode(holder,bit);
			printf("value: %c\n",holder->bit);
			if(holder->size > 0)
			{
				size = holder->size;
				printf("size: %d\n",size);
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
				DCValue = numberFinder(patternHolder,size);
				printf("number: %d",DCValue);
				 dcFlag = 1;

				 // ac code comes here

				break;
			}
		}
		break;
	} 
	return 0;
}


/*


Read the byte data and get as 1s and 0s read and write it to 
a file as x x ie leaving spaces so that reading is easy
*/