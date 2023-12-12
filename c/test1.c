#include <stdio.h>
int main ()
{
    int m = 0, n = 0;
    scanf (" %d", &m);
    scanf (" %d", &n);
    int i = 0, j= 0;
    i = (n*n*n)+(6*n*n)+12*n;
    printf ("i = %d\n", i);
    j = i/m;
    printf ("j = %d\n", j);
   return 0;
}
