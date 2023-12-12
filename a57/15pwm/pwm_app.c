#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <ctype.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
#define FATAL  do {fprintf (stderr, "error at line%d, file %s (%d) [%s]\n", __LINE__, __func__, errno, strerror(errno)); exit (1); }while (0)
#define PWM_ENABLE_REG "0x2807e020"
#define PWM_ENABLE_VAL "0xff"
#define GPIO_PAD_MODE_PWM0_REG "0x32b30050"
#define GPIO_PAD_MODE_PWM0_VAL "0x141"
#define GPIO_PAD_MODE_PWM2_REG "0x32b30060"
#define GPIO_PAD_MODE_PWM2_VAL "0x141"
#define PWM0_CONTROLLER "0"
#define PWM0_CHANNEL0    "0"
#define PWM2_CONTROLLER "2"
#define PWM2_CHANNEL0   "2"
#define WRITE_TYPE_H  "h"//半字 2个字节
#define WRITE_TYPE_B  "b"
#define WRITE_TYPE_W  "w"//字   4个字节
#define PWM_PERIOD    "1000000000"
#define PWM_DUTY1     "0"
#define PWM_DUTY2     "1000000"
enum PWMX
{
    PWM0,
    PWM2,
    PWM0_PWM2,
    PWMMAX
};

static int pwm_config (char *attr, const char *val, int node);

//将指定的类型的数据写入由addr映射的虚拟地址中
//type指定访问内存的类型 
//addr 传入的物理地址
int gpio_mode (char *addr, char *type, char *data)
{
    off_t target;
    int fd;
    int access_type = 'w';
    void *map_base, *virt_addr;
    unsigned long writeval;
    //获取访问内存的物理地址
    target = strtoul (addr, 0, 0);//将字符串转换成unsigned long
    //获取数据类型
    access_type = tolower (type[0] );//将字符转换为小写字母
    //访问内存
    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC))<0)
    {   
        FATAL;
    }
    //映射一页大小到内存
    map_base = mmap (0,MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target&~MAP_MASK);
    if (map_base == (void *)-1);
    {
        FATAL;
    }
    //计算虚拟地址
    virt_addr = map_base+(target & MAP_MASK);
    writeval = strtoul (data, 0, 0);
    switch (access_type)
    {
        case 'b':
            *((unsigned char *)virt_addr) = writeval;
            break;
        case 'h':
             *((unsigned short *)virt_addr) = writeval;
            break;
        case 'w':
             *((unsigned int *)virt_addr) = writeval;
            break;

    }
    //解除映射
    if (munmap (map_base, MAP_SIZE) == -1)
    {
        FATAL;
    }
    close (fd);
    return 0;
}  static char pwm0_path[128];
    static char pwm2_path[128];

int main (int argc, char **argv)
{
    enum PWMX pwmx = PWMMAX;//默认=3
      int fd, len;
    //判断参数是否合法
    if (argc!=2)
    {
        printf ("usage:pwm <num>\n");
        printf ("num:0, 2, 0_2\n");
        exit (-1);
    }
    else 
    {
        if (strcmp(argv[1], "0") ==0)
        {
            pwmx = PWM0;
        }
        else if (strcmp(argv[1],"2")==0)
        {
            pwmx = PWM2;
        }
        else if (strcmp(argv[1],"0_2")==0)
        {
            pwmx = PWM2;
        }else
        {
             printf ("usage:pwm <num>\n");
             printf ("num:0, 2, 0_2\n");
             exit (-1);
        }

    }
    //配置管脚模式
    //使能pwm控制器
    gpio_mode (PWM_ENABLE_REG, (char *)WRITE_TYPE_B, PWM_ENABLE_VAL);
    //pwm0 pwm2按照手册进行配置
    gpio_mode (GPIO_PAD_MODE_PWM0_REG, (char *)WRITE_TYPE_H, GPIO_PAD_MODE_PWM0_VAL);
    gpio_mode (GPIO_PAD_MODE_PWM2_REG, (char *)WRITE_TYPE_H, GPIO_PAD_MODE_PWM2_VAL);
    //将pwm0 pwm2的路径导出到pwm0/pwm2下
    sprintf (pwm0_path, "/sys/class/pwm/pwmchip%s/pwm%s", PWM0_CONTROLLER, PWM0_CHANNEL0);
    sprintf (pwm2_path, "/sys/class/pwm/pwmchip%s/pwm%s", PWM2_CONTROLLER, PWM2_CHANNEL0);
    //判断有没有访问权限
    if (access(pwm0_path, F_OK))//有访问权限返回1
    {
        if(fd = open ("/sys/class/pwm/pwmchip0/export", O_WRONLY) <0)
        {

            perror ("open failed\n");
            exit (-1);
        }
        len = strlen (PWM0_CHANNEL0);
        if (len!=write (fd, PWM0_CHANNEL0, len))
        {

            perror ("write failed\n");
            close (fd);
            exit (-1);
        }
        close (fd);
    }
   if (access(pwm2_path, F_OK))//有访问权限返回0
    {
        if ((fd = open ("/sys/class/pwm/pwmchip2/export", O_WRONLY)) <0)
        {

            perror ("open failed\n");
            exit (-1);
        }
        len = strlen (PWM2_CHANNEL0);
        if (len!=write (fd, PWM2_CHANNEL0, len))
        {
            perror ("write failed\n");
            close (fd);
            exit (-1);
        }
        close (fd);
    }
//如果可以正常导出，可以调节占空比
//配置pwm0,pwm2的持续周期为1s
    if (pwm_config ("period", PWM_PERIOD, 0) <0)
    {
        perror ( "config pwm0 duty1 attribute failed\n");
        exit(-1);
    }
    //初始化占空比为0
    if (pwm_config ("duty_cycle", PWM_DUTY1, 0)<0)
    {
        fprintf (stderr, "coonfig pwm0 duty failed\n");
        exit (-1);
    }
    //使能pwm0
    if (pwm_config ("enable", "1", 0)<0)
    {
        fprintf (stderr, "coonfig pwm0 enable failed\n");
        exit (-1);
    }
    //pwm2
    if (pwm_config ("period", PWM_PERIOD, 1) <0)
    {
        perror ( "config pwm2 duty1 attribute failed\n");
        exit(-1);
    }
    //初始化占空比为0
    if (pwm_config ("duty_cycle", PWM_DUTY1, 1)<0)
    {
        fprintf (stderr, "coonfig pwm2 duty failed\n");
        exit (-1);
    }
    //使能pwm2
    if (pwm_config ("enable", "1", 1)<0)
    {
        fprintf (stderr, "coonfig pwm1 enable failed\n");
        exit (-1);
    }
    int step;
    char increase = 1;
    char dutys[128];
    unsigned int duty1 = atoi (PWM_DUTY1);
    unsigned int duty2 = atoi (PWM_DUTY2);
    unsigned int duty = duty1;
    step = (int)((duty2 - duty1)/100);
    printf ("duty1= %d  duty2= %d  step= %d\n", duty1, duty2, step);
    while (1)
    {
        if (increase)
        {
            duty += step;
        }
        else if (increase == 0)
        {
            duty -=step;
        }
        memset (dutys, 0, sizeof (dutys)/sizeof(dutys[0]));
        sprintf (dutys,"%d", duty);
        //根据输入，决定使用哪个pwm,决定使用一路pwm后，将另一个pwm占空比调到最大
        if (pwmx == PWM0)
        {                                     
            if (pwm_config ("duty_cycle", PWM_DUTY2, 1)<0)//pwm2调成最大占空比
            {
                fprintf (stderr, "config pwm duty error\n");
                exit (-1);
            }
            if (pwm_config ("duty_cycle", dutys, 0) <0)
            {
                fprintf (stderr, "config pwm duty error\n");
                exit (-1);
            }
        }
        else  if (pwmx == PWM2)
        {
            if (pwm_config ("duty_cycle", PWM_DUTY2, 0)<0)
            {
                fprintf (stderr, "config pwm duty error\n");
                exit (-1);
            }
            if (pwm_config ("duty_cycle", dutys, 1) <0)
            {
                fprintf (stderr, "config pwm duty error\n");
                exit (-1);
            }
        }
        //当输入0-2
        else if (pwmx == PWM0_PWM2)
        {
            if (pwm_config ("dutu_cycle", dutys, 0) <0)
            {
                fprintf (stderr, "config pwm duty error\n");
                exit (-1);
            }
            if (pwm_config ("dutu_cycle", dutys, 1) <0)
            {
                fprintf (stderr, "config pwm duty error\n");
                exit (-1);
            }  
        }
        if (duty >= duty2)
        {
            increase = 0;
        }
        if (duty <= duty1)
        {
            increase = 1;
        }
        usleep (2000);

    }

    return 0;
}
static int pwm_config (char *attr, const char *val, int node)
{
    char file_path [256];//存储pwm设备节点属性文件的路径 
    int fd, len;
    if (node == 0)
    {
        if ((strlen (pwm0_path  )+ strlen (attr))>(sizeof(file_path)/sizeof(char)))
        {
            fprintf (stderr, "file_patj is too long\n");
            return -1;
        }
        sprintf (file_path, "%s%s", pwm0_path, attr);

    }
    else if (node == 1)
    {
        if ((strlen (pwm2_path  )+ strlen (attr))>(sizeof(file_path)/sizeof(char)))
        {
            fprintf (stderr, "file_patj is too long\n");
            return -1;
        }
        sprintf (file_path, "%s%s", pwm2_path, attr);

    }
    if ((fd = open (file_path, O_WRONLY)) <0)
    {
        perror ("open error\n");
        exit (-1);
    }
    len = strlen (val);
    if (len!=write (fd, val, len ))
    {
        perror ("write error\n");
        exit (-1);
    }
    close (fd);
    return 0;


}
