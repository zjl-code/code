#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//按键的可变信息
struct gpio_desc
{
    unsigned int group//gpio控制器
    unsigned int num[16];
};
static void gpio_init (void);
static unsigned int gpio_get(struct gpio_desc gpios); 
static int gpio_write (char *pptr, char *attr, char *val);

static struct gpio_desc gpio_val[6];

int main (int argc, char *argv[])
{
    struct gpio_desc gpios;
    char gpio_num[6];//组号+ 管脚编号
    char pptr[128];
    int fd = 0, len, gpio_fd;
    char buff[256];
    struct pollfd fds[1];//poll监听
    //参数判断
    //close (gpio_fd);
    if ((argc!=3) || (atoi(argv[1])!=4) || ((atoi(argv[2])!=1&&atoi(argv[2])!=3)))
    {
        printf ("usage :./key_app <gpio group> <gpio_num>\n");
        printf ("key1: ./key_app 4 1 \n");
        printf ("key2: ./key_app 4 3 \n");
        exit (0);
    }
    //gpio管脚的初始化
    gpio_init ();
    gpios.group = atoi(argv[1]);//按键所对应的gpio的组号
    gpios.num[0] = atoi(argv[2]);//所对应gpio组里的gpio管脚编号
    sprintf (gpio_num," %d",  gpio_get (gpios));//将gpios中的编号存放到gpio_num中
    sprintf (pptr, "/sys/class/gpio/gpio%s\n", gpio_num);//将路径存放到pptr
    if (access (pptr, F_OK))//检查用户对文件是否有权限，文件是否存在，成功返回0,失败返回-1
    {
        //访问设备文件      读写设备文件
        //配置中断处理方式
        if ((fd = open ("/sys/class/gpio/export", O_WRONLY)) < 0)
        {
            perror ("open failed\n");
            exit (-1);
        }
        //导出文件
        len = strlen (gpio_num);//组号和管脚号的整合
        if (len != write (fd, gpio_num, len))//得到gpio编号对应的设备文件eg:gpio428
        {
            perror ("write error\n");
            close (fd);
            exit (-1);
        }
    }
        //操作具体设备文件的属性文件
        //配置中断的触发方式--->双沿
        if (gpio_write (pptr, "edge", "both"))
        {
            perror ("config gpio`s edge attribute error\n");
            exit (-1);
        }
        //配置方向为输入
        if (gpio_write (pptr, "direction", "in"))
        {
            perror ("config gpio`s direction attribute failed\n");
            exit (-1);

        }
        //打开设备文件,监听文件描述符
        memset (buff, 0, sizeof(buff)/sizeof(buff[0]));
        sprintf (buff, "%s%s", pptr, "value");//将具体gpio设备文件下的value属性文件路径写到buff中
        gpio_fd = open (buff, O_RDONLY);
        if (gpio_fd == -1)
        {
            perror ("gpio open failed\n");
            close (gpio_fd);
            exit (-1);
        }
        fds[0].fd = gpio_fd;//fds是结构体polled的实例化对象，fd是其中的成员
        fds[0].events = POLLPRI;//表示等待高优先级可读
        int ret = read (gpio_fd, buff, 10);//从gpio_fd中读到buff中
        if (ret == -1)
        {
            perror ("gpio read error\n");
            close (gpio_fd);
            exit (-1);
        }
        printf ("please press button\n");
        while (1)
        {
            ret = poll (fds, 1, -1);//poll监听(监听fd,监听1个，超时时间为无限超时)
            if (ret == -1)
            {
                perror ("poll error");
            }
            if (fds[0].revents &POLLPRI)//结果不为0说明发生了POLLPRI事件--->中断发生
            {
                ret = lseek (gpio_fd, 0, SEEK_SET);
                if (ret == -1)
                {
                    close (gpio_fd);
                    perror ("lseek ");
                }
                memset (buff, 0, sizeof(buff)/sizeof(char));
                ret = read(gpio_fd, buff, 10);
                if (ret == -1)
                {
                    perror ("read error");
                    close (gpio_fd);
                }
                printf ("eky interrupt event\n");
                printf ("key value: %s\n",buff);

            }
            
        }

    close (gpio_fd);
    return 0;
}

void gpio_init ()
{
    int i, j;
    for (i = 0; i<6; i++)
    {
        gpio_val[i].group = i;
        for (j = 0; i<16; j++)
        {
            gpio_val[i].num[j] = 496-16*i+j;

        }
    }
}

static unsigned int gpio_get(struct gpio_desc gpios) 
{
    
    return gpio_val[gpios.group].num[gpios.num[0]];//将组号和管脚号整合到一起返回
}
static int gpio_write (char *pptr, char *attr, char *val)
{
    char file_path[256];
    int fd,  len;
    sprintf (file_path, "%s%s", pptr, attr);
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


