#include <stdio.h>
#include <string.h>

int main ()
{
   char src[20], dest[13];

   strcpy(src, "Thiswhat");
   strcpy(dest, "This is de");
   strncat(dest, src, 4);

   printf("最终的目标字符串： |%d|", strlen(dest));
   
   return(0);
}