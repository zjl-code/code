#include <stdio.h>
//输入正整数判断有几位数 
int main()
{
	char a[80];
	int i=0,y=0;
	printf("输入一个数字：");
	gets(a);
	for(i=0;a[i]!='\0';i++)
	{
		if(a[i]>='0'&&a[i]<='9')
			y++;
	}
	puts(a);
	printf(" %s是 %d位数\n",a,y);
	return 0;
}
