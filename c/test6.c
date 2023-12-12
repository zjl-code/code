#include <stdio.h>
#define SQR(x) x*x
int main (int argc, char *argv[])
{
   // int a =1;
   float a = 1;
    int k=2,m=1;
    a/=SQR(k+m)/SQR(k+m);
    printf ("SQR = %d\n",SQR(k+m));
    printf ("SQR/SQR = %d\n",SQR(k+m)/SQR(k+m));
    printf ("%f\n",a);
}
