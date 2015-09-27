#include <stdio.h>
#include <stdlib.h>


#define symbol 10

struct nodes
{
	int pixel;
	float probabilty;
	struct nodes *left,*right;
};

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

void printPreOrder(struct nodes *tree)
{
	if(tree)
	{
		printf("pixel: %d probabilty: %f \n",tree->pixel,tree->probabilty);
		printPreOrder(tree->left);
		printPreOrder(tree->right);
	}
}

void binaryTreeConstruction(struct nodes *head,struct nodes *array, int count)
{
	struct nodes *holder1,*holder2;
	while(count - 1) // remaining two
	{
		holder1 = createNode();
		holder2 = createNode();
		*holder1 = array[count - 2];
		*holder2 = array[count - 1];
		array[count - 2].probabilty = holder1->probabilty + holder2->probabilty;
		array[count -2 ].pixel = 10000;
		array[count - 2].left = holder1;
		array[count - 2].right = holder2;
		count--;
		qsort(array,count,sizeof(struct nodes),compare_function);	
	}
		head->probabilty = array[count].probabilty + array[count - 1].probabilty;
		head->left = &array[0];
		head->right = &array[1];
}


int main(int argc, char const *argv[])
{
	int i,j,count;
	int inputSize = 10;
	struct nodes *node,*head;
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
	node = (struct nodes *) malloc(sizeof(struct nodes) * count);
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

	return 0;
}

// read about tree traversal
// and huffman code generation