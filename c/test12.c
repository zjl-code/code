#include <stdio.h>
#include <string.h>
char *mystrcpy (char *dest, char *src)
{
	int i;
	int n = strlen(src);
	for(i = 0; i<n; i++)
	{
		if(src[i]!='\0')
		{
			dest[i] = src[i];
		}
	}
	dest[i]='\0';
	printf("%s\n",dest);
}
int main (void)
{
	char a[] = "zhang";
	char b[] = "jiale";
	mystrcpy (a, b);
	return 0;
}
