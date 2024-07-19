#include <stdio.h>
#include <stdlib.h>

#define ROW 21//行
#define COL 18//列
#define NR 4
static char gameArea[ROW][COL]={0};
char *state="\033[32m游戏中...\033[0m";
static unsigned int level=0;
static unsigned int score=0;
static unsigned int maxscore=0;

static int startx=7,starty=6;//方块出现的起始位置
static int type=0;//方块当前类型
static int nextType=0;//方块的下一种类型
static int diret=0;//方块的方向

int bl[nextType][diret][NR][NR];

void initalgamearea(void)
{
	int i;
	//屏幕设置
	printf("033\[2J");//清屏
	system("stty -icanon");//关缓冲
	system("stty -echo");//关回显
	fprintf(stdout,"\033[?25I");//关闭鼠标显示
	//初始化行
	for(i=0;i<COL;i++)
	{
		gameArea[0][i]=8;
		gameArea[5][i]=8;
		gameArea[ROW-1][i]=8;
	}
	//初始化列
	for(i=0;i<ROW;i++)
	{
		gameArea[i][0]=8;
		gameArea[i][COL-1]=8;
	}
	//初始化中间一小列
	for(i=1;i<5;i++)
	{
		gameArea[i][6]=8;
	}
}
void drawBlock(char block[NR][NR])
{
	int x,y;
	//画当前方块
	for(y=0;y<NR;y++)
	{
		for(x=0;x<NR;x++)
		{
			if(block[y][x]!=0)
			{
				gameArea[starty+y][startx+x]=block[y][x];

			}

		}
	}

	//画下一个方块
	for(x=0;x<2;x++)
	{
		for(y=0;y<NR;y++)
		{
			if(bl[nextType][0][x][y]!=0)
				gameArea[3+x][2+y]=bl[nextType][0][x][y];
			else
				gameArea[3+x][2+y]=0;
		}
	}
}
//显示游戏
void showGame(void)
{
	int i, j;
	//定位到第一行第一列
	fprintf(stdout,"\033[1;1H");
	fflush(stdout);
	//打印所有数据
	for(i=0;i<ROW;i++)
	{
		for(j=0;j<COL;j++)
		{
			if(gameArea[i][j]==0)
			{
				fprintf(stdout," ");
			}
			else if(gameArea[i][j]==8)
			{
				fprintf(stdout,"\033[04m \033[0m");
			}
			else
			{
				fprintf(stdout,"\033[%dm[]\033[0m",gameArea[i][j]+40);
			}
		}
		fputc('\n',stdout);
	}
	//打印提示信息
	fprintf(stdout,"\033[2;3H\033[33m『下一个』\033[0m\n");
	fprintf(stdout,"\033[2;15H当前级别:\033[36m%u\033[0m\n",level);
	fprintf(stdout,"\033[3;15H当前分数:\033[32m%u\033[0m\n",score);
	fprintf(stdout,"\033[4;15H最高纪录:\033[35m%u\033[0m\n",maxscore);
	fprintf(stdout,"\033[5;15H当前状态:%s\n",state);

}
int main()
{
	          
	//初始化功能区域
	initalgamearea();
	//画方块
	drawBlock(bl[type][diret]);
	//显示游戏
	showGame();
	//恢复缓冲
	system("stty icanon");
	//恢复回显
	system("stty echo");
	while(1);

	return 0;
}
