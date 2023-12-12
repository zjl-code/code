#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct student
{
    unsigned char flag;
    int id;
};
int calculate (int a)
{
    int b, c = 5;
    float f =  2.3 , *q = NULL;
    double d = 0.2;
    q = (float*)malloc (sizeof (float));
    *q = f+ d;
   if (a == 1)
    {
        a = (a-b)*f/(rand()-c)+*q;
    }
    else 
    {
        a = sqrt (rand() - c);
     }
}
void getStuId (struct student *stuData)
{
    if (stuData->flag == 0x100)
    {
        stuData->id +=500;
    }
    else
    {
        stuData->id +=300;
    }

}
int main ()
{
    int a_Main = 2;
    printf ("4\n");
    struct student stuData;
    
    calculate (a_Main);
   
    stuData.id = 20;
    getStuId(&stuData);
    printf ("Id= %d\n",stuData.id);
    printf ("\n%d", a_Main);

    return 0;
}
