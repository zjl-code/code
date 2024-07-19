#include <stdio.h>
int add(int a,int b)
{
	return a+b;
}
int mul(int a,int b)
{
	return a*b;
}
 int fun(int (*p)(int,int),int m,int n)
{
	int c=p(m,n);
	return c;
}
int main()
{
#if 0
	int a[2][3]={1,2,3,4,5,6};
	int (*p)[3]=a;
	printf("%p\n",&a+1);
	printf("%p\n",&a);
#endif
	int a=11,b=22;
	int c=fun(mul,a,b);
	printf("%d\n",c);
	return 0;
}
