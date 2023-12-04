#include <stdio.h>
int main (void)
{
    int a = 0x12345678;
    char *p =(char *)&a;
    if (*p == 0x78)
    {
        printf ("xiaoduan\n");
    }
    else
        printf ("da\n");
    return 0;
}
