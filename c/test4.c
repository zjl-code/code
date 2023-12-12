#include <stdio.h>
#include <stdlib.h>

int main (void)
{
    int a=10,b,c,d;
    b=a++;;c=++a;d=10*a++;
    printf ("a=%d b=%d  c=%d d=%d \n",a,b,c,d);
    return 0;
}
#if 0
#define STRING_1 "STRING_1"
#define STRING_2 "STRING_2"

char ** getstrings (int *pnum)
{
    *pnum = 2;
    //char *pstring[] = {STRING_1, STRING_2};//在栈上分配空间
    char **pstring = malloc (sizeof (char *) * (*pnum));//在堆上分配空间
    pstring[0] = STRING_1;
    pstring[1] = STRING_2;
    return pstring;
}
int main ()
{
    int num ;
    char **p = NULL;
    p = getstrings (&num);
    printf ("%s\n", p[0]);
    free (p);
    return 0;
}
#endif
