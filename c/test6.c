#include <stdio.h>
#define SQR(x) x*x


union
{
    char x[5]; 
    int i;
}a;
void main ()
{
    a.x[0] = 10;
    a.x[1] = 1;
    printf ("%d, %d\n",a.i, sizeof (a));
    a.x[2] = 1;
    printf ("%d\n",a.i);
 }

#if 0
int main (int argc, char *argv[])
{
#if 0
   // int a =1;
   float a = 1;
    int k=2,m=1;
    a/=SQR(k+m)/SQR(k+m);
    printf ("SQR = %d\n",SQR(k+m));
    printf ("SQR/SQR = %d\n",SQR(k+m)/SQR(k+m));
    printf ("%f\n",a);
#endif
    union V //v和x公用一块空间
    {
        struct X
        {
             unsigned char s1:2;//占2位00--->0
            unsigned char s2:3;//占3位001--->1
            unsigned char s3:3;//占3位011--->3
        }x;
        unsigned char c;
    }v;
    v.c = 100;//二进制0110 0100
    printf ("%d, %d, %d\n", v.x.s1, v.x.s2, v.x.s3);
}
#endif
