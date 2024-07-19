#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//输出原数并逆序打印 
void rever_number(int a)
{
	if(a>=0&&a<=9)
	{
		printf("%d",a);
	}
	else
	{
		printf("%d",a%10);
		rever_number(a/10);
	}
}
int main()
{
	int a;
	int b[2][3]={1,2,3,4,5,6};
	printf("%d\n",sizeof(b));
	printf("输入一个正整数：");
	scanf("%d",&a);
	printf("%d ",a);
	rever_number(a);
	return 0;
}
