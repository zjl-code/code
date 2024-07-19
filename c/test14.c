#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main()
{
	char a[32];
	int i=0;
	int j=0, k=0, l=0, m=0;//字母、空格、数字、其他字符
	printf("输入一行字符:");
	gets(a);
	int n=strlen(a);//字符长度
	for(i=0;i<n;i++)
	{
		if((65<=a[i]&&a[i]<=90)||(97<=a[i]&&a[i]<=122))
		{
			j+=1;
		}
		else if(a[i]==32)
		{
			k+=1;
	    }
		else if(48<=a[i]&&a[i]<=57)
		{
			l+=1;
		}
		else
		{
			m+=1;
		}

	}
	printf("字母的个数:%d\n",j);
	printf("空格的个数:%d\n",k);
	printf("数字的个数:%d\n",l);
	printf("其他字符数:%d\n",m);

	return 0;
}
