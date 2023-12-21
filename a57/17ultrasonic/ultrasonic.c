#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>

struct gpio_desc
{
    unsigned int group;
    unsigned int num[16];
};
enum SRF05_IO
{
    TRIG,
    ECHO,
    NONE
};
struct gpio_desc gpio_val[6];
static int SRF05_trigger (struct gpio_desc gpios);
static int SRF05_open_echo (struct gpio_desc gpio);
static int SRG05_get_distance (int echo_fd);

static void gpio_init (void)
{
    int i, j;
    for (i = 0; i<6; i++)
    {
        gpio_val[i].group = i;
    for (j = 0; j<16; j++)
    {
        gpio_val[i].num[j]  = 496-i*16+j;
    }

}

}
static int gpio_get (struct gpio_desc gpio)
{
    return gpio_val[gpio.group].num[gpio.num[0]];
};
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

//gpio设备文件 导出到用户空间，然后根据传入的type将gpio4_4配置为输出 gpio4_5输入
static int SRF05_if_set (struct gpio_desc gpios, enum SRF05_IO type ) 
{
    char pptr[128];
    char gpio_num[6];
    int fd, len;
    sprintf (gpio_num, "d", gpio_get(gpios));
    sprintf (pptr, "/sys/class/gpio/gpio%s", gpio_num);
    if (access (pptr, F_OK))
    {   
        if (0>(fd = open ("/sys/class/gpio/export", O_WRONLY)))
        {
        perror ("open failed \n");
        return  -1;
        }
         len = strlen(gpio_num);
        if (len != write (fd, gpio_num, len))
        {   
            perror ("write failed\n");
            close (fd);
            return -1;
        }
    }
//trig out gpio4_4
    if (type == TRIG)
    {
        if (gpio_write (pptr, "direction", "out"))
        {
            fprintf (stderr, "config gpio direction failed\n");
            return -1;
        }
        if (gpio_write (pptr, "edge", "none"))
        {
            fprintf (stderr, "config gpio edge failed\n");
            return -1;
         }
        if (gpio_write (pptr, "value", "0"))//配制成低电平
        {
             fprintf (stderr, "config gpio value failed\n");
            return -1;
        }

    }
// echo gpio4_5  in
    else if (type == ECHO)
    {
        if (gpio_write (pptr, "direction", "in"))
        {
            fprintf (stderr, "config gpio direction failed\n");
            return -1;
        }
        if (gpio_write (pptr, "edge", "both"))
        {
            fprintf (stderr, "config gpio edge failed\n");
            return -1;
        }
    

    }

    return 0;
}
int main (int argc, char **argv)
{

    struct gpio_desc gpios;
//init
    gpio_init ();
//trig gpio4_4--->436输出
    gpios.group = 4;
    gpios.num[0] = 4;
    int echo_fd;
    //trig out > dircetion  none
    if (SRF05_if_set (gpios, TRIG)<0)
    {
        fprintf (stderr, "triggrt gpio init failed\n  ");
        exit (-1);
    }
    gpios.group = 4;
    gpios.num[0] = 5;
    //echo in > /dircetion  both
    if (SRF05_if_set (gpios, ECHO)<0)
    {
        fprintf (stderr, "triggrt gpio init failed\n  ");
        exit (-1);
    }
    //打开gpio4_5对应的设备文件
    if (echo_fd = SRF05_open_echo (gpios) == -1)
    {
        perror ("open echo failed\n");
        exit (-1);
    }
    while (1)
    {
        //trigger send
        gpios.group = 4;
        gpios.num[0] = 4;
        //发出信号(产生10us高电平),并开始计时
        SRF05_trigger (gpios);
        //计算距离  发出信号时间 接受信号时间
        SRG05_get_distance (echo_fd);
        
        usleep (100000);//100ms
    }
    exit (0);

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
static int SRF05_get_distance (int echo_fd)
{
    char buf[128];
    struct pollfd fds[1];//定义一个struct polled对象
    fds[0].fd = echo_fd;
    fds[0].events = POLLPRI;//检测异常（中断是异常的一种）
    struct timespec ts, te;
    float distance;
    int ret = read (echo_fd, buf, 10 );
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
                }
                break;
            }
        }
    }
    return 0;
}

