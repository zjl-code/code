#include <stdio.h>

typedef struct X
{
    char a; 
    double b;
    int c;
}x;
void change (int *a, int &b, int c)
{
    c = *a;
    b = 3;
    *a = 2;
}
int main (void)
{
    int a = 1,b = 2, c = 3;
    int d[] = {1,2,3,4,5};
    int *ptr;
    change (&a, b, c);
    ptr = (int *)(&d+1);
    printf ("%5d %5d\n", *(d+1), *(ptr-1));

    return 0;
}
