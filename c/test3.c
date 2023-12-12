#include <stdio.h>
#include <string.h>
char *GetMemory (void)
{
    char *p = "hello world";
    return p;
}
int main (void)
{
  
    char string[10];
    char *str1 = "0123456789";
    char str2[] ={0,1,2,3,4,5,6,7,8,9};
     char str3[] ={'0','1','2','3','4','5','6','7','8','9'};
    char str4[] ="0123456789";//长度为11
    #if 0
    printf ("str2 = %d\n",strlen(str2)) ;
    printf ("str4 = %d\n",strlen(str4));
    #endif
   #if 0
    printf ("str4 = %d\n",sizeof (str4));
    printf ("str3 = %d\n",sizeof (str3));
    printf ("str2 = %d\n",sizeof (str2));
    #endif
    strcpy (string, str1);
    int i = 0;
    for (i = 0;i <10; i++)
    {
        printf ("  %c",string[i]);
     }
     printf ("\n");

     #if 0
     1.16
     char *str = NULL;
     str = GetMemory ();
     printf (str);
     #endif
    return 0;
}
