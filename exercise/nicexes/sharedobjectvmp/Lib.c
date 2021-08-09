#include <stdio.h>
#include <unistd.h>

void foobar(int i) {
    printf("Pringting form Lib.so %d\n", i);
    sleep(-1);
}