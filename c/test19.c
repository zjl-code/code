#include <stdio.h>
int removeDuplicates(int* nums, int numsSize)
{
	int p=0,q=1;
	while(q<numsSize)
	{
		
		if(nums[p]==nums[q])
		{
			q++;
		}
		else if(nums[p]!=nums[q])
		{
			nums[p+1]=nums[q];
			q++;
			p++;
		}

	}
		return p+1;

}
int main()
{
	int nums[]={0,0,1,1,2,2,3,3,4,5};
	int numsize=sizeof(nums)/sizeof(nums[0]);
	int a=removeDuplicates(nums, numsize);
	printf("%d\n",a);
	return 0;
}
