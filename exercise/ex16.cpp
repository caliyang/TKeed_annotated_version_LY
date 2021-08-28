#include <stdio.h>
#include <sys/types.h>

int main()
{
    unsigned int temp=0x01ff02fe;
	printf("%d\n", &temp);
	unsigned int address;
	scanf("%d", &address);
	printf("%d\n", *((unsigned char *)address));

    return 0;
}