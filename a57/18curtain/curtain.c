#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
/*
*电机gpio4_11,gpio5_12,gpio5_13,gpio5_14
*超声波gpio4_4,gpio4_5
*/
enum SRF05_IO
{
    TRIG,
    ECHO,
    NONE
};
struct gpio_desc 
{
    unsigned int group;
    unsigned int num[16];

};
struct gpio_desc gpio_val[6];
static void gpio_init (void)
{
    int i = 0, j = 0;
    for (i = 0; i<6; i++)
    {
         gpio_val[i].group = i;
        for (j = 0; j< 16;  j++)
        {
            gpio_val[i].num[j] = 496-16*+j;
        }
    }
    return ;
}
static int gpio_get (struct gpio_desc gpios)
{
    return gpio_val[gpios.group].num[gpios.num[0]];
}
static int gpio_write (char *pptr, char *attr, char *value)
{
    char file_path[256];
    int fd, len;
    sprintf (file_path, "%s%s", pptr, attr);
    if ((fd = open (file_path, O_WRONLY)) <0)
    {
        perror ("open sttribute failed\n");
        return -1;
    }
    len = strlen (value);
    if (len != write (fd, value, len))
    {
        perror ("write attribute failed\n");
        close (fd);
        return -1;
    }
    close (fd);
    return 0;
}
static int step_moter_set ()
{
    char pptr[256]; //存放路径
    int i = 0, j = 0, fd, len;
    for (i = 4; i<6; i++)
    {
        gpio_val[i].group = i;
        if (i = 4)
        {
            int j = 11;
            gpio_val[i].num[j] = 496- 16*i +j;
            
        }
        else if (i == 5)
        {
            for (j = 12; j<15; j++)
            {
                gpio_val[i].num[j] = 496 -16*i +j;
            }

        }
        memset (pptr, 0,  sizeof(pptr)/sizeof(char));
        sprintf (pptr, "/sys/class/gpio/gpio%d",gpio_val[i].num[j]);
        if (access (pptr, F_OK))//文件不存在
        {
            if (fd = open ("/sys/class/gpio/expoprt", O_WRONLY) <0)
            {
                perror ("open step_moter attribute failed\n");
                return -1;
            }
            len = strlen ((char*)&gpio_val[i].num[j]);
            if (len != write (fd ,(char *)&gpio_val[i].num[j], len))
            {
                perror ("write step_moter attribute failed\n");
                close (fd);
                return -1;
            }
            
        }
        //配置为4个管脚为输出模式，没有中断触发，初始为低电平
        if (gpio_write (pptr, "edge", "none"))//错误-1
        {
            fprintf (stderr, "config gpio edge error\n");
            exit (-1);
        }
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
 }
static int SRF05_set (struct gpio_desc gpios, enum SRF05_IO type)
{
    char pptr[128];//存放路径
    int fd, len;
    char gpio_num[6];//存放4_4 4_5转换后的编号
    sprintf (pptr, "/sys/class/gpio/gpio%d", gpio_get(gpios));
    if (access (pptr, F_OK))
    {
        if ( (fd =open ("/sys/class/gpio/export", O_WRONLY)) <0)
        {
            perror ("open failed\n");
            return -1;
        }
        len = strlen(gpio_num);
        if (len != write (fd, gpio_num,len))
        {
            perror ("write failed\n");
            close (fd);
            return -1;
        }
       
    }
    if (type == TRIG)
    {
        if (gpio_write (pptr, "direction", "out"))
        {
            fprintf (stderr, "config trig direction failed\n");
            return -1;
        }
        if (gpio_write (pptr, "edge", "none"))
        {
            fprintf (stderr, "config trig edge failed\n");
            return -1;
        }
        if (gpio_write (pptr, "value", "0"))
        {
            fprintf (stderr, "config trig value failed\n");
            return -1;
        }

    }
    else if(type == ECHO)
    {
        if (gpio_write (pptr, "direction", "in"))
        {
            fprintf (stderr, "config echo direction failed\n");
            return -1;
        }
        if (gpio_write (pptr, "edge", "both"))
        {
            fprintf (stderr, "config echo edge failed\n");
            return -1;
        }

    }


    return 0;
}
static int SRF05_open_echo (struct gpio_desc gpios)
{
    char pptr[128];
    char gpio_num[6];
    char fp[256];
    int echo_fd  = -1;
    sprintf (gpio_num, "%d", gpio_get(gpios));
    sprintf (pptr, "/sys/class/gpio/gpio%s", gpio_num);
    memset (fp, 0, sizeof(fp)/sizeof(fp[0]));
    sprintf (fp, "%s%s", pptr, "value");
    echo_fd = open (fp, O_RDONLY);
    return echo_fd;
}
static int SRF05_trigger (struct gpio_desc gpios)
{
    char pptr[128];
    char gpio_num[6];
    struct timespec ts, te;
    sprintf (gpio_num, "d", gpio_get(gpios));
    sprintf (pptr, "/sys/class/gpio/gpio%s", gpio_num);
    if (gpio_write (pptr, "value", "1"))
    {
        perror ("gpio_write 1 failed\n");
        return -1;
    }
    //计时10us
    clock_gettime (CLOCK_REALTIME, &ts); //获取当前实时时间到ts中
   do
   {
         clock_gettime (CLOCK_REALTIME, &te);
   } while ((te.tv_nsec - ts.tv_nsec)/1000 <= 10);

   if (gpio_write (pptr, "value", "0"))
    {
        perror ("gpio_write 1 failed\n");
        return -1;
    }

    return 0;
}
static int SRF05_get_distance (int echo_fd)
{
    char buf[128];
    struct pollfd fds[1];//定义一个struct polled对象
    fds[0].fd = echo_fd;
    fds[0].events = POLLPRI;//检测异常（中断是异常的一种）
    struct timespec ts, te;
    float distance;
    int ret = read (echo_fd, buf, 10 );//读取echo文件的value属性文件
    if (ret == -1)
    {
        perror ("gpio read error\n");
        return -1;
    }
    while (1)
    {
        ret = poll (fds, 1, -1);
        if (ret == -1)
        {
            perror ("poll failed\n");
            return -1;
        }
        if (fds[0].revents & POLLPRI)//中断信号到来
        {
            ret = lseek (echo_fd, 0, SEEK_SET);
            if (ret == -1)
            {
                perror ("lseek failed\n");
                return -1;
            }
            memset (buf, 0, sizeof(buf)/sizeof(buf[0]));
            ret = read (echo_fd, buf, 1);
            if (ret == -1)
            {
                perror ("ead failed\n");
                return -1;
            }
            if (atoi(buf) == 1)
            {
                clock_gettime (CLOCK_REALTIME, &ts);
            }
            else if (atoi(buf) == 0)
            {
                clock_gettime (CLOCK_REALTIME, &te);
                distance = ((te.tv_nsec - ts.tv_nsec)/1000/29.4);
                if (distance >0)
                {
                    printf ("distance = %.2f cm\n", distance);
                    return distance;
                }
                break;
            }
        }
    }
    return 0;
}
int main (int argc, char **argv)
{
    struct gpio_desc gpios;
    int echo_fd;
    //init
    gpio_init ();
    //电机--->输出  
    step_moter_set ();

    //超声波4_4out  trig    4_5in  echo
    gpios.group = 4;
    gpios.num[0] = 4;//trig --->out
    SRF05_set (gpios, TRIG);
    gpios.group = 4;
    gpios.num[0] = 5;//echo --->in
    SRF05_set (gpios, ECHO);
    //打开gpio4_5对应的设备文件的value文件检测中断
    if (echo_fd = SRF05_open_echo (gpios) == -1)
    {
        perror ("open echo failed\n");
        exit (-1);
    }
    while (1)
    {
        int i, j;
        char pptr[256];
        //trigger send
        gpios.group = 4;
        gpios.num[0] = 4;
        //发出信号(产生10us高电平),并开始计时
        SRF05_trigger (gpios);

        //检测距离当距离<特定值(5)时电机停止转动
        int distance = SRF05_get_distance (echo_fd);
        if (distance > 5)
        {
            //电机转动
            for (i = 4; i<6; i++)
           {
                gpio_val[i].group = i;
                 if ( i = 4)
                {
                    int j = 11;
                    gpio_val[i].num[j] = 496- 16*i +j;
                    
                 }
                else if (i == 5)
                {
                    for (j = 12; j<15; j++)
                    {
                        gpio_val[i].num[j] = 496 -16*i +j;
                    }

                }
                
                memset (pptr, 0, sizeof(pptr)/sizeof(char));
                sprintf (pptr, "/sys/class/gpio/gpio%d", gpio_val[i].num[j]);
                gpio_write (pptr, "value", "1");//启动电机
                //时间间隔太小可能会烧，太大可能转的很慢
                usleep (10000);//1ms=10^3us;
                memset (pptr, 0, sizeof(pptr)/sizeof(char));
                sprintf (pptr, "/sys/class/gpio/gpio%d", gpio_val[i].num[j]);
                gpio_write (pptr, "value", "0");
            }
        }
        else if (distance <=5)
        {
            for (i = 5; i>3; i--)
           {
                gpio_val[i].group = i;
                 if ( i = 4)
                {
                    int j = 11;
                    gpio_val[i].num[j] = 496- 16*i +j;
                    
                 }
                else if (i == 5)
                {
                    for (j = 14; j>11; j--)
                    {
                        gpio_val[i].num[j] = 496 -16*i +j;
                    }

                }
                
                memset (pptr, 0, sizeof(pptr)/sizeof(char));
                sprintf (pptr, "/sys/class/gpio/gpio%d", gpio_val[i].num[j]);
                gpio_write (pptr, "value", "1");//启动电机
                //时间间隔太小可能会烧，太大可能转的很慢
                usleep (10000);//1ms=10^3us;
                memset (pptr, 0, sizeof(pptr)/sizeof(char));
                sprintf (pptr, "/sys/class/gpio/gpio%d", gpio_val[i].num[j]);
                gpio_write (pptr, "value", "0");
            }
 
        }
        
    
    }

    return 0;
}
