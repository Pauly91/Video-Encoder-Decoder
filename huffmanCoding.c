#include <stdio.h>
#include <stdlib.h>


#define symbol 10

struct nodes
{
	int pixel;
	float probabilty;
	//char code;
	struct nodes *left,*right;
};

struct HuffmanTable
{
	int pixel;
	int *code;
	int length;
};

void printPreOrder(struct nodes *tree)
{
	if(tree)
	{
		printf("pixel: %d probabilty: %f \n",tree->pixel,tree->probabilty);
		printPreOrder(tree->left);
		printPreOrder(tree->right);
	}
}
void printCode(struct HuffmanTable huffmanTable)
{
	int i;
	for (i = 0; i < huffmanTable.length; ++i)
	{
		printf("%d",huffmanTable.code[i]);
	}
}


void codeGeneration(struct nodes *root,struct HuffmanTable *huffmanTable, int *code, int count, int *tableCount)
{
    // Assign 0 to left edge and recur
    printf("\ntableCount:%d\n",*tableCount);
    int i;
    if (root->left)
    {
        code[count] = 0;
        printf("\nCode: Left:");
        for (i = 0; i < count+1; ++i)
        {
        	printf("%d",code[i]);
        }
        printf("\npixel: %d probabilty: %f \n",root->left->pixel,root->left->probabilty);
        codeGeneration(root->left, huffmanTable, code, count + 1, tableCount);
    }
 
    // Assign 1 to right edge and recur
    if (root->right)
    {
        code[count] = 1;
        printf("\nCode: Right:");
        for (i = 0; i < count+1; ++i)
        {
        	printf("%d",code[i]);
        }
        printf("\npixel: %d probabilty: %f \n",root->right->pixel,root->right->probabilty);
        codeGeneration(root->right, huffmanTable, code, count + 1, tableCount);
    }
 

    if (!root->left && !root->right)
    {
        printf("\nCode: leaf:");
        for (i = 0; i < count; ++i)
        {
        	printf("%d",code[i]);
        }		
		huffmanTable[*tableCount].pixel = root->pixel;
		huffmanTable[*tableCount].code = (int *) malloc(sizeof(int) * count);
		for (i = 0; i < count; ++i)
		{
					printf("-- > i:%d code:%d",i,code[i]);
					huffmanTable[*tableCount].code[i] = code[i];
		}

		huffmanTable[*tableCount].length = count;
		    printf("\ntableCount:%d\n",*tableCount);
		(*tableCount)++;
		    printf("\ntableCount:%d\n",*tableCount);
		for (i = 0; i < count; ++i)
		{
			printf("\npixel: %d: code: ",huffmanTable[i].pixel);//,huffmanTable[i].code);
			printCode(huffmanTable[i]);
		}
    }
}

int compare_function(const void *a,const void *b) 
{
	struct nodes *x = (struct nodes *) a;
	struct nodes *y = (struct nodes *) b;
	if (x->probabilty < y->probabilty) 
		return 1;
	else if (x->probabilty > y->probabilty) 
		return -1; 
	return 0;
}

struct nodes *createNode()
{
	struct nodes *node;
	node = (struct nodes *) malloc(sizeof(struct nodes *));
	return node;
}



void binaryTreeConstruction(struct nodes *head,struct nodes *array, int count)
{
	int i;
	struct nodes *holder1,*holder2;
	while(count - 2) // remaining two
	{
		holder1 = createNode();
		holder2 = createNode();
		*holder1 = array[count - 2];
		*holder2 = array[count - 1];
		array[count - 2].probabilty = holder1->probabilty + holder2->probabilty;
		array[count - 2].pixel = 10000;// dummy value
		array[count - 2].left = holder1;
		array[count - 2].right = holder2;
		count--;
		qsort(array,count,sizeof(struct nodes),compare_function);	
		printf("\n\nSorted\n");
		for (i = 0; i < count; ++i)
		{
			printf("pixel: %d probabilty:%f \n",array[i].pixel,array[i].probabilty);
		}	
		printf("\n\nCount:%d\n",count);
	}
		head->probabilty = array[0].probabilty + array[1].probabilty;
		head->pixel = 10000;// dummy value
		head->left = &array[0];
		head->right = &array[1];
}



int main(int argc, char const *argv[])
{
	int i,j,count;
	int inputSize = 10;
	int *code = NULL;
	int codeCount = 0, tableCount = 0;
	struct nodes *node,*head = NULL;
	struct HuffmanTable *huffmanTable;
	char input[10] = {2,7,4,7,3,5,6,7,2,9};
	int hashMap[symbol] = {0};
	for (i = 0; i < inputSize; ++i)
	{
		hashMap[input[i]] ++ ;
	}
	for (i = 0; i < symbol; ++i)
	{
		printf("%d:%d\n",i,hashMap[i]);
	}
	for (i = 0,count = 0; i < symbol; ++i)
	{
		if(hashMap[i] > 0)
			count++;
	}
	printf("count:%d\n",count);
	// printf("Working1\n");
	code = (int *) malloc(sizeof(int) * count);
	node = (struct nodes *) malloc(sizeof(struct nodes) * count);
	huffmanTable = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable) * count);
	head = malloc(sizeof(struct HuffmanTable));
	for (i = 0,j = 0; i < symbol; ++i)
	{
		if(hashMap[i] > 0)
		{
			node[j].pixel = i;
			node[j].probabilty = ((float)hashMap[i]/symbol);
			node[j].left = node[j].right = NULL;
			j++;
		}
	}
	for (i = 0; i < count; ++i)
	{
		printf("pixel: %d probabilty:%f \n",node[i].pixel,node[i].probabilty);
	}
	qsort(node,count,sizeof(struct nodes),compare_function);	
	printf("Sorted\n");
	for (i = 0; i < count; ++i)
	{
		printf("pixel: %d probabilty:%f \n",node[i].pixel,node[i].probabilty);
	}	
	binaryTreeConstruction(head,node,count);
	printPreOrder(head);
	printf("\nTest Huffman Table\n");
	for (i = 0; i < count; ++i)
	{
		printf("\npixel: %d: code: ",huffmanTable[i].pixel);//,huffmanTable[i].code);
		printCode(huffmanTable[i]);
	}

	codeGeneration(head, huffmanTable, code, codeCount, &tableCount);
	printf("\nHuffman Table\n");
	for (i = 0; i < count; ++i)
	{
		printf("\npixel: %d: code: ",huffmanTable[i].pixel);//,huffmanTable[i].code);
		printCode(huffmanTable[i]);
	}

	return 0;
}

// read about tree traversal
// and huffman code generation