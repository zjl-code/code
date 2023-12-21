#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if 0
union 
{
    char x[5];
    int i;
}a;

void main ()
{
    a.x[0] = 10;
    a.x[1] = 1;
    printf ("%d %d\n", a.i, sizeof(a));
}

int main ()
{
    int a, b, c, d;
    a=10;b=a++;c=++a;
    d=10*a++;
    printf ("a %db %d c%d d%d\n", a,b,c,d);
}
#endif


char *Getmeory (void)
{
    char *p = (char *) malloc (6); // 动态分配6字节的内存空间，包括字符串"hello"和字符串结束符'\0'
    strcpy(p, "hello"); // 将字符串"hello"复制到动态分配的内存空间中
    return p;
}

int main ()
{
    char *str = NULL;
    str = Getmeory ();
    printf ("str:%s\n", str);
    free (str); // 释放动态分配的内存空间
    return 0;
}

