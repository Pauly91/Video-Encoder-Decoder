#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TableSize 12


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
int main(int argc, char const *argv[])
{
	FILE *fp = NULL;
	int i;
	struct HuffmanDCTable *huffmanDCTable;

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
	treeCreator(head,huffmanDCTable,0,0);
	printf("\n**PreOrder**\n");
	printPreOrder(head);	

	return 0;
}