#include <stdio.h>
/*
   *int nums[]={4,9,1,7,3,0,8}; 需求找到第k大个元素并返回下标，eg:第2大的元素是8,下标是6
   */
void get_kmax (int nums[], int size, int k, int index[])
{
	//进行数组的边界判断
	if (k <= 0 || k >= size)
	{
		printf ("K超出数组范围\n");
	
	}
	int i = 0, j = 0;
	
	//存放数组下标
	for (i = 0; i<size; i++)
	{
		index[i] = i;
	}
	//使用冒泡排序进行排序
	for(i = 0; i<size-1; i++)
	{
		for (j = 0; j<size-i-1; j++)
		{
			if (nums[j] > nums[j+1])
			{
				int temp = nums[j];
				nums[j] = nums[j+1];
				nums[j+1] = temp;
				//交换数组下标
				int temp_index = index[j];
				index[j] = index[j+1];
				index[j+1] = temp_index;
			}
		}
	}
}
int main()
{
	int nums[]={4,9,1,7,3,0,8};
	int size = sizeof(nums)/sizeof(nums[0]);//size = 7
	int k = 0;
	int index[size];
	printf ("请输入k的值\n");
	scanf ("%d", &k);
	get_kmax(nums, size, k, index);
	printf ("第%d大的元素是%d，下标是%d\n",k, nums[size - k], index[size - k]);
	return 0;
}
