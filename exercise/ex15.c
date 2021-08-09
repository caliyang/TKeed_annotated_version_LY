#include <stdio.h>
#include <stdlib.h>

int aaaaa = 1;
static int ddddd = 4;

//int b = 2;

int test(int a , int b)
{
	return a+b;
}


int main()
{
	const int bbbbb = 2;
	static int ccccc = 3;
	printf("%d", test(aaaaa,bbbbb));
	return 0;
}