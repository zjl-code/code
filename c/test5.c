#include <stdio.h>
#include <string.h>
char* str_reverse (char *str1)
{
    char str2[32];
    int size = strlen (str1);//5,用sizoeof=6;
    printf ("%d\n",size);
    int i =0, j = 0;
    for (i = size-1; i>=0; i--)
    {
        str2[j] = str1[i];
        j++;
    }
    str2[j] = '\0'; 
    printf ("%s\n", str2);
    return NULL;
}
char *str_cpoy (char *des, char *source, int n)
{
    int i = 0, j = 0;
    for (i=0;i<n;i++)
    {
        if(source[i]!='\0')
        {
            des[j] = source[i];
            j++;
        }
    }
    des[j] = '\0';

    return des;
}
int str_cmp (char *str1, char *str2)
{
    int n = strlen(str1);
    int i=0, j=0;
    for(i=0;i<n;i++)
    {
        if (str1[i]==str2[j])
        {
            j++;
        }
        else
        {
            return str1[i]-str2[j];
        }
       
    }

    return 0;
}
int main (void)
{
    char str1[] = "hello";
    char str2[32];
    char *a;
    int ret = 0;
    str_reverse (str1);
    a = str_cpoy (str2, str1, strlen(str1));
    printf ("%s\n",a);
    ret = str_cmp (str1, str2);
    printf ("ret = %d\n",ret);
    return 0;
}

