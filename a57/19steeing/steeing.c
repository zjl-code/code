#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>       
#include <stdlib.h>
//定义uart端口
enum uart_port
{
    COMPORT1 = 1,
    COMPORT2 = 2,
    COMOPORT_MAX
};
enum steering_cmd
{
    VERSION,
    SPEED0,
    SPEED7,
    DEGREE0,
    DEGREE90,
    DEGREE180,
    POS_READ,
    STAT_READ,
    BUT_READ,
    LED_ON,
    LED_OFF,
    STEERING_MAX,
};
unsigned char steering_control[]={
	0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0a,     //读取软件版本号
	0x01, 0x06, 0x00, 0x02, 0x00, 0x00, 0x28, 0x0a,     //设置舵机速度0（最慢）
	0x01, 0x06, 0x00, 0x02, 0x00, 0x07, 0x69, 0xC8,     //设置舵机速度7（最快）
	0x01, 0x06, 0x00, 0x06, 0x01, 0xf4, 0x69, 0xdc,     //舵机转到0度（位置值500）
	0x01, 0x06, 0x00, 0x06, 0x05, 0xdc, 0x6b, 0x02,     //舵机转到90度（位置值1500）
	0x01, 0x06, 0x00, 0x06, 0x09, 0xc4, 0x6e, 0x08,     //舵机转到180度（位置值2500）
	0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b,     //读舵机的位置
	0x01, 0x03, 0x00, 0x0a, 0x00, 0x01, 0xa4, 0x08,     //读所有舵机运动状态
	0x01, 0x03, 0x00, 0x0b, 0x00, 0x01, 0xf5, 0xc8,     //读按钮自上次读取后是否被按下过
	0x01, 0x06, 0x00, 0x0c, 0x00, 0x01, 0x88, 0x09,     //设置用户LED灯开
	0x01, 0x06, 0x00, 0x0c, 0x00, 0x00, 0x49, 0xc9,     //设置用户LED灯关
};
//校验方式
enum check_event
{
    ODD,
    EVEN,
    NONE,
    EVENT_MAX
};
//定义uart的核心数据结构
struct uart_desc
{
    enum uart_port port;
    unsigned int baudrate;
    unsigned char nbits;//数据位个数
    unsigned char stopbits;//停止位个数
    enum check_event event;//校验方式

};
static int uart_open (int com);
static int uart_setting (int fd, struct uart_desc uart);
static int steering_ctrl (int fd, char *outdata, enum steering_cmd cmd);

int main (int argc, char **argv)
{
    struct uart_desc uart;
    uart.port = COMPORT2;    
    int fd;
    char buf[8];
    short ret = 0;
    if (argc < 3 || atoi(argv[1])!= uart.port)
    {
        printf ("usage: ./uart_app 2 <旋转角度%s>\n ", argv[2]);
        exit (-1);
    }
    //打开设备文件
    fd = uart_open (uart.port);
    if (fd < 0)
    {
        perror ("uart_open failed\n");
        exit (-1);
    }
        //配置uart2的参数115200 8 N 1
   uart.baudrate = 115200;//SG90决定
   uart.nbits = 8;
   uart.event = NONE;//没有校验位
   uart.stopbits = 1;

   if ((uart_setting (fd, uart))<0)//串口的基本信息配置完毕
   {
        perror ("uart_setting error\n");
        exit (-1);
   }
   //------------------------------------------------------------------

   //   读软件版本号
   steering_ctrl (fd, buf, VERSION);//将版本号从fd中读到buf中
   printf ("version:V%d.%d\n\n",buf[4], buf[5]);
   printf ("please press button3\n");
   usleep (100000);//100ms                                                        
   //通过按键控制舵机进行转动，按键按下舵机转动
   //读取舵机运动状态，buf[4] << 8 | buf[5]!=0--->舵机可用
   while (ret == 0)
   {
        steering_ctrl (fd, buf, BUT_READ);
        ret = (short)buf[4] << 8 | buf[5];//ret!=0舵机可用
        printf ("1.0 0x%x\n", ret);
        usleep (100000);
   }
   printf ("steering  is  active\n");
   //舵机可用
            //打开led
    steering_ctrl (fd, buf, LED_ON);
    usleep (100000);//100ms
    //根据用户输入的信息舵机旋转
    if (strcmp (argv[2], "90") == 0)
    {
        steering_ctrl (fd, buf, DEGREE90);
    }
    else if (strcmp (argv[2], "180") == 0)
    {
        steering_ctrl (fd, buf, DEGREE180);
    }
    else 
    {
        steering_ctrl (fd, buf, DEGREE0);
    }
    usleep (100000);//100ms
    //等待运动完成并且读取状态
    ret = 0xffff;
    while (ret !=0)
    {
        steering_ctrl (fd, buf, STAT_READ);//读取运动状态
        ret = (short) (buf[4] <<8 | buf[5]);
        printf ("1.2 0x%x\n", ret );
        usleep (100000);
    }
    //让舵机旋转到0
    steering_ctrl (fd, buf, DEGREE0);
    usleep (100000);

    ret = 0xffff;  //再次读取运动状态值
    while (ret !=0)
    {
        steering_ctrl (fd, buf, STAT_READ);
        ret = (short) (buf[4] <<8 | buf[5]);
        printf ("1.2 0x%x\n", ret );
        usleep (100000);
    }
    //关闭led
    steering_ctrl (fd, buf, LED_OFF);
    usleep (100000);

    return 0;
}
//打开对应设备文件，返回fd
static int uart_open (int com)
{
    char uart2_dev[128];
    int fd;
    sprintf (uart2_dev, "/dev/ttyAMA%d", com);
    switch (com)
    {
        case COMPORT1:
        //非阻塞方式打开文件
            fd = open (uart2_dev, O_RDWR| O_NOCTTY| O_NDELAY);
            if (fd <0)
            {
                perror (" open serial device failed\n");
                return -1;
            }
            break;
        case COMPORT2:
             fd = open (uart2_dev, O_RDWR| O_NOCTTY| O_NDELAY);
             if (fd <0)
            {
                perror (" open serial device failed\n");
                return -1;
            }

            break;

    }
    if (fcntl (fd, F_SETFL, 0) < 0)//设置文件属性,将文件状态标志设置为0
    {
        perror ("fcntl failed\n");
        return -1;
    }
    //测试stdin是不是一个终端设备
   if (isatty (STDIN_FILENO) == 0)
   {
        printf ("stdin is not a terminal devices\n");
   }
       return fd ;
}
static int uart_setting (int fd, struct uart_desc uart)
{
    //将串口原始属性保存到oldtio
    struct termios oldtio, newtio;
    if(tcgetattr (fd, &oldtio) !=0 )//得到串口属性存放到oledtio中
    {
        perror ("Setup serial failed\n");
        return -1;

    }
     bzero (&newtio, sizeof(newtio)); 
     newtio.c_cflag |= CREAD | CLOCAL;//？设置了newtio结构体的c_cflag字段，将CREAD CLOACL标志位置1,CREAD表示启用接收功能，CLOCAL表示忽略调制解调状态器状态线
     newtio.c_cflag &= ~ CSIZE;//？将数据位清除，使用默认数据位大小

     //设置数据位个数
     switch (uart.nbits)
     {
        case 7:
            newtio.c_cflag |= CS7;//使用7位数据位
            break;
        case 8:
            newtio.c_cflag |= CS8;//使用8位数据位
            break;

     }
     //设置奇偶校验
     switch (uart.event)
     {
        case ODD:
            newtio.c_cflag |= PARENB;//启用输入输出奇偶校验
            newtio.c_cflag |=PARODD;//使用奇校验
            newtio.c_iflag |= (INPCK |ISTRIP);//INPCK启用输入奇偶校验,ISTRIP去掉第8位

            break;
        case EVEN:
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &=~(PARODD);//使用偶校验
            newtio.c_iflag |= (INPCK |ISTRIP);
            break;
        case NONE:
            newtio.c_cflag &=~ PARENB;//不校验
            break;
        case EVENT_MAX:
            printf ("check bit setting error\n");
            return -1;


     }
     //波特率
     switch (uart.baudrate)
     {
        case 2400:
           cfsetispeed (&newtio, B2400);
           cfsetospeed (&newtio, B2400);
           break;
        case 4800:
           cfsetispeed (&newtio, B4800);
           cfsetospeed (&newtio, B4800);
           break;
        case 9600:
           cfsetispeed (&newtio, B9600);
           cfsetospeed (&newtio, B9600);
           break;
           case 57600:
           cfsetispeed (&newtio, B57600);
           cfsetospeed (&newtio, B57600);
           break;
        case 19200:
           cfsetispeed (&newtio, B19200);
           cfsetospeed (&newtio, B19200);
           break;
          case 115200:
           cfsetispeed (&newtio, B115200);
           cfsetospeed (&newtio, B115200);
           break;
        case 460800:
           cfsetispeed (&newtio, B460800);
           cfsetospeed (&newtio, B460800);
           break;
     }

     //停止位个数
    if (uart.stopbits == 1)
    {
        newtio.c_cflag &=~(CSTOPB);

    }
    else if (uart.stopbits == 2)
    {
        newtio.c_cflag |= CSTOPB;//CSTOPB设置2位停止位
    }
    //轮询读
    newtio.c_cc[VMIN] = 0;//非规范读取的最小字符数 ？
    newtio.c_cc[VTIME] = 0;//读取超时时间 ？
    tcflush (fd, TCIFLUSH);
    //将设置好的串口参数设置到fd中
     if (tcsetattr (fd, TCSANOW, &newtio)!=0)//TCSANOW改变立即生效
     {
        perror ("com set failed\n");
        return -1;

     }
     return 0;
}
static int steering_ctrl (int fd, char *outdata, enum steering_cmd cmd)
{
    int ret ;
    switch (cmd)
    {
        case VERSION:
            ret = write (fd, &steering_control[0*8], 8);//将版本号写到fd（设备文件）中
            break;
        case SPEED0:
             ret = write (fd, &steering_control[1*8], 8);
            break;

        case SPEED7:
             ret = write (fd, &steering_control[2*8], 8);
            break;

        case DEGREE0:
             ret = write (fd, &steering_control[3*8], 8);
            break;

        case DEGREE90:
             ret = write (fd, &steering_control[4*8], 8);
            break;

        case DEGREE180:
             ret = write (fd, &steering_control[5*8], 8);
            break;

        case POS_READ:
             ret = write (fd, &steering_control[6*8], 8);
            break;

        case STAT_READ:
             ret = write (fd, &steering_control[7*8], 8);
            break;

        case BUT_READ:
             ret = write (fd, &steering_control[8*8], 8);
            break;

        case LED_ON:
             ret = write (fd, &steering_control[9*8], 8);
            break;

         case LED_OFF:
             ret = write (fd, &steering_control[10*8], 8);
            break;

        default:
            break;
    

    }
    if (ret == 8)
    {
        usleep (5000);
        ret = read (fd, outdata, 8);//从fd中读数据到outdata中
        if (ret !=8)
        {
            printf ("read failed\n");

        }
    }
    return ret;
}

