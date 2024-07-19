#include <stdio.h>
#include <string.h>
void rmov (char a[])
{
	int i = 0, j = 0;
	int n = strlen(a);
	for (i = 0, j = 0; i<n; i++)
	{
		if(a[i] != 32)
		{
			a[j] = a[i];
			j++;		
		}
	}
	a[j] = '\0';
	printf ("%s\n", a);

}
int main()
{
	char a[] = "zhang jia le";
	rmov(a);
	return 0;
}
