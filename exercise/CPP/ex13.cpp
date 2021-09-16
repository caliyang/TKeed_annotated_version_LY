#include <iostream>
#include <stdlib.h>

int main() {
    int i;
    char *p = (char *)malloc(10);
    char *pt = p;
    for(int i; i < 10; i++) p[i] = 'z';
    delete p;
    pt[1] = 'x';
    free(pt);
    return 0;
}