#include <stdio.h>
#include <stdlib.h>
//节点
typedef struct node
{
	int data;
	struct node*next;
}lnode;
//链表
typedef struct list
{
	lnode head;
	int count;
}llist;
void init_list(llist *list)
{
	list->head.next=&(list->head);
	list->count=0;
}
lnode *newnode(int data)
{
	lnode *new=malloc(sizeof(lnode));//申请节点空间
	if(new==NULL)
	{
		return NULL;
	}
	new->next=NULL;
	new->data=data;
	return new;
}
int head_insert_node(llist *list,lnode *node)//头插
{
	if(list==NULL||node==NULL)
	{
		return -1;
	}
	node->next=list->head.next;
	list->head.next=node;
	list->count++;
	return 0;
}
lnode* head_delt_node(llist *list)//头删
{
	lnode *node=NULL;
	if(list==NULL||list->head.next==NULL)
	{
		return NULL;
	}
	node=list->head.next;
	list->head.next=node->next;
	node->next=node;//可有可无
	list->count--;
	return node;
}
void insert_node(llist *list, lnode *node1, lnode*node2)//node2插入到node1后
{
	if(list==NULL||node1==NULL||node2==NULL)
	{
		return ;
	}
	lnode *p=list->head.next;
	for(;p!=&(list->head);p=p->next)
	{
		if(p==node1)
		{
			node2->next=p->next;
			node1->next=node2;
			list->count++;
		}

	}
}
lnode *delete_node(llist *list,lnode *node1)//指定位置删除节点
{
	if(list==NULL||node1==NULL)
		return NULL;
	lnode *p=list->head.next;
	for(;p!=&(list->head);p=p->next)
	{
		
	}
	if(p->next==node1)
	{
		p->next=node1->next;
		node1->next=NULL;
		list->count--;
		return node1;
	}
	else
	{
		return NULL;
	}
}
int print_list(llist *list)
{
	lnode *p=list->head.next;
	while(p!=&(list->head))
	{
		printf(" %d ",p->data);
		p=p->next;
	}
	printf("\n");
	return 0;
}
int main()
{
	llist list_link;//声明链表对象
	lnode *new=NULL;
	int data=0;
	//初始化链表
	init_list(&list_link);
	for(int i=0;i<5;i++)
	{
		scanf("%d",&data);
		new=newnode(data);//新建节点
		//头部插入节点
		head_insert_node(&list_link,new);
	}
	print_list(&list_link);
	printf("\n");
	new=newnode(88);
	insert_node(&list_link,list_link.head.next,new);//指定位置插入
	print_list(&list_link);
	printf("\n");
	lnode *delet_node=head_delt_node(&list_link);	//删除节点
	if(delet_node!=NULL)
	{
		free(delet_node);
	}
	//打印链表
	print_list(&list_link);
	printf("\n");
	delet_node=delete_node(&list_link,list_link.head.next);//指定位置删除
	print_list(&list_link);
	printf("\n");

	return 0;
}
