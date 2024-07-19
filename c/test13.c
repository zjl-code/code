#include <stdio.h>
 
int main()
{
	//printf("%d",putchar(97));
	//putchar('a');
	//越界问题
#if 0
	char c=300;
	printf("%d\n",c);
#endif
	//最大公约最小公倍
	int  a,b;
	printf("输入两个数字:");
	scanf("%d %d",&a,&b);
	int t;
	if(b>a)
	{
		t=b;b=a;a=t;
	}
	int y;//余数
	while(b!=0)
	{
		y=a%b;
		a=b;
		b=y;
	}
	printf("最大公约数%d\n",a);
	return 0;
}
