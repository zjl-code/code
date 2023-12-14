#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//管脚的组号和编号4_11 5_12 5_13 5_14
/*
*432+11-->443--->gpio4_11
*416+12--->428--->gpio5_12
*
*/
struct gpio_desc 
{
    unsigned int group;
    unsigned int num;
};
struct gpio_desc gpio_val[6];
static void gpio_init ()
{
    int i = 0;
    gpio_val[0].group = 4;
    gpio_val[0].num = 432+11;
    for (i = 5; i<8; i++)
    {
        gpio_val[i-4].group = 5;
        gpio_val[i-4].num = 416+i+7;
    }
}
static int gpio_write (char *pptr, char *attr, char *val)
{
    char file_path[256];
    int fd,  len;
    sprintf (file_path, "%s%s", pptr, attr);//eg:具体gpio428下的属性文件
    if (fd = open (file_path, O_WRONLY)<0)
    {
        perror ("open failed\n");
        return fd;

    }
    len = strlen (val);
    if (len!=write (fd, val, len))
    {
        perror ("write failed\n");
        close (fd);
        return -1;
    }
    close (fd);
    return 0;

}
//将已经初始化好的gpio编号转存到gpios
static void gpio_get (struct gpio_desc *gpios, int num)
{
    gpios->group = gpio_val[num].group;
    gpios->num = gpio_val[num].num;
    
}

int main (int argc, char ** argv)
{
    int  i = 0, fd, len;
    char pptr[128];//存放路径
    char gpio[4];//存放gpios.num
    struct gpio_desc gpios;
    //init
    gpio_init ();
    for (i = 0; i<4; i++)
    {
        //获取gpio管脚编号
        gpio_get (&gpios, i);//将gpio_val[i]的编号存到gpios中

       //将gpio节点路径保存到pptr
        memset (pptr, 0, sizeof(pptr)/sizeof(char));
        sprintf (pptr, "/sys/class/gpio/gpio%d", gpios.num);
        //判断该文件有没有访问权限
        if (access (pptr, F_OK))
        {
            if (0>(fd =open ("/sys/class/gpio/export", O_WRONLY)))
            {
                perror ("open failed\n");
                exit (-1);
            }
            //将gpios中的编号放到gpio数组（字符）中
            sprintf (gpio, "%d", gpios.num);
            len = strlen (gpio);
            if (len != write (fd, gpio, len))//得到编号gpio的gpio的设备文件eg:gpio428
            {
                perror ("write failed\n");
                close (fd);
                exit (-1);
            }

            close (fd);
           
        }
        //向不同的设备节点中写入不同的值.配置gpio管脚
        //配置为轮循方式，没有中断触发方式
        if (gpio_write (pptr, "edge", "none"))//错误-1
        {
            fprintf (stderr, "config gpio edge error\n");
            exit (-1);
        }
        //管脚配置为输出模式
        if (gpio_write (pptr, "direction", "out"))//错误-1
        {
            fprintf (stderr, "config gpio direction error\n");
            exit (-1);
        }
        //默认管脚输出低电平--->电机不转
         if (gpio_write (pptr, "value", "0"))//错误-1
        {
            fprintf (stderr, "config gpio value error\n");
            exit (-1);
        }

    }
    //每隔10ms轮流输出高电平
    while (1)
    {
        for (i = 0; i<4; i++)
        {
            gpio_get (&gpios, i);
            memset (pptr, 0, sizeof(pptr)/sizeof(char));
            sprintf (pptr, "/sys/class/gpio/gpio%d", gpios.num);
            gpio_write (pptr, "value", "1");//启动电机
            //时间间隔太小可能会烧，太大可能转的很慢
            usleep (10000);//1ms=10^3us;
            memset (pptr, 0, sizeof(pptr)/sizeof(char));
            sprintf (pptr, "/sys/class/gpio/gpio%d", gpios.num);
            gpio_write (pptr, "value", "0");

        }

    }

    return 0;
}
