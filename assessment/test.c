#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int main(int argc, char const *argv[])
{
	int a = 4;
	float b = 1.2;
	float result_float;
	int result_int;
	result_int = round(a * b);
	result_float = a * b;
	printf("result_int : %d result_float :%f on the go: %d\n",result_int,result_float,round(a*b));
	return 0;
}