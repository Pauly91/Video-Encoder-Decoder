#include <stdio.h>
#include <stdlib.h>





int main(int argc, char const *argv[])
{
	unsigned char block[8][8] = {
					{16,11,	10,	16,	24,	40,	51,	 61},
					{12,12,	14,	19,	26,	58,	60,	 55},
					{14,13,	16,	24,	40,	57,	69,	 56},
					{14,17,	22,	29,	51,	87,	80,	 62},
					{18,22,	37,	56,	68,	109,103, 77},
					{24,35,	55,	64,	81,	104,113, 92},
					{49,64,	78,	87,	103,121,120,101},
					{72,92,	95,	98,	112,100,103, 99}
	};

	unsigned char reblock[0][0];
	int i = 0, j = 0, holder = 0;
	int blocksize = 8;
	FILE *fp = NULL;

	if((fp = fopen("zigzag","w")) == NULL)
	{
		printf("File not open\n");
		return -1;
	}
	while( i != 8 && j != 8)
	{

		if(j%2 == 0 && i == 0)
		{
			fprintf(fp,"%u\n",block[i][j]);
			j++;
		}
		else if(j%2 == 1 && i == 0)
		{
			while(j != 0)
			{
				fprintf(fp,"%u\n",block[i][j]);
				j -= 1;
				i += 1;
			}
		}
		else if(i%2 == 0 && j == 0)
		{
			while(i != 0)
			{
				fprintf(fp,"%u\n",block[i][j]);
				j += 1;
				i -= 1;
			}			
		}
		else if(i%2 == 1 && j == 0)
		{
			if(i == blocksize - 1)
			{
				
				fprintf(fp,"%u\n",block[i][j]);
				j++;
			}

			else
			{
				fprintf(fp,"%u\n",block[i][j]);
				i++;
			}

		}
		else if(i == blocksize - 1 && j%2 == 1)
		{
			holder = j;
			while(i != holder)
			{
				fprintf(fp,"%u\n",block[i][j]);
				j += 1;
				i -= 1;				
			}
		}
		else if(i == blocksize - 1 && j%2 == 0)
		{
				fprintf(fp,"%u\n",block[i][j]);
				j++;
		}		
		else if(j == blocksize - 1 && i%2 == 1)
		{
			fprintf(fp,"%u\n",block[i][j]);
			i++;
		}
		else if(j == blocksize - 1 && i%2 == 0)
		{
			holder = i;
			while(j != holder)
			{
				fprintf(fp,"%u\n",block[i][j]);
				j -= 1;
				i += 1;				
			}
		}
		if(i == blocksize -1 && j == blocksize - 1)
		{
				fprintf(fp,"%u\n",block[i][j]);
				i++;
				j++;
				break;
		}

	}
	fclose(fp);

	if((fp = fopen("zigzag","r")) == NULL)
	{
		printf("File not open\n");
		return -1;
	}

	i = 0;
	j = 0;

	while( i != 8 && j != 8)
	{

		printf("i:%d j:%d\n",i,j);
		if(j%2 == 0 && i == 0)
		{
			fscanf(fp,"%u",&reblock[i][j]);
			printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
			j++;
		}
		else if(j%2 == 1 && i == 0)
		{
						printf("--> i:%d j:%d %u \n",i,j,reblock[i][j]);	
			while(j != 0)
			{
				fscanf(fp,"%u",&reblock[i][j]);
				printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
				j -= 1;
				i += 1;
			}
		}
		else if(i%2 == 0 && j == 0)
		{
			while(i != 0)
			{
				fscanf(fp,"%u",&reblock[i][j]);
				printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
				j += 1;
				i -= 1;
			}			
		}
		else if(i%2 == 1 && j == 0)
		{
			if(i == blocksize - 1)
			{
				
				fscanf(fp,"%u",&reblock[i][j]);
				printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
				j++;
			}

			else
			{
				fscanf(fp,"%u",&reblock[i][j]);
				printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
				i++;
			}

		}
		else if(i == blocksize - 1 && j%2 == 1)
		{
			holder = j;
			while(i != holder)
			{
				fscanf(fp,"%u",&reblock[i][j]);
				printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
				j += 1;
				i -= 1;				
			}
		}
		else if(i == blocksize - 1 && j%2 == 0)
		{
			fscanf(fp,"%u",&reblock[i][j]);
			printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
			j++;
		}		
		else if(j == blocksize - 1 && i%2 == 1)
		{
			fscanf(fp,"%u",&reblock[i][j]);
			printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
			i++;
		}
		else if(j == blocksize - 1 && i%2 == 0)
		{
			holder = i;
			while(j != holder)
			{
				fscanf(fp,"%u",&reblock[i][j]);
				printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
				j -= 1;
				i += 1;				
			}
		}
		if((i == blocksize -1) && (j == blocksize - 1))
		{
			fscanf(fp,"%u",&reblock[i][j]);
			printf("i:%d j:%d %u \n",i,j,reblock[i][j]);
			i++;
			j++;
			break;
		}

	}
	printf("Output\n");
	for (i = 0; i < blocksize; ++i)
	{
		for (j = 0; j < blocksize; ++j)
		{
			printf("%u \n",reblock[i][j]);

		}
	}
	fclose(fp);

	return 0;
}