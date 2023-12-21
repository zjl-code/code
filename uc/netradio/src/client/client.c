#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <proto.h>
#include "client.h"
/*
*-P--port  指定接收端口
*-M--mgroup 指定多播组
*-p--player 制定播放器  
-H--help 显示帮助
*/
struct client_conf_St client_conf = 
{
    .rcvport = DEFAULT_RCVPORT,
    .mgroup = DEFAULT_MGROUP,


};
struct ip_mrequ mrequ;
static void printhelp (void)
{
    pritnf ("-P--port  指定接收端口\n  -M--mgroup 指定多播组\n  -p--player 制定播放器\n  -H--help 显示帮助\n")；
} 
static ssize_t writen (int fd, const char *buf, size_t len)
{
    pos = 0;
    while (len > 0)
    {
        ret = write (fd, buf+pos, len);
        if (ret < 0)
        {
            if (errno ==EINTR)
                continue;
            perror ("write ()");
            exit (1);
        }
        len -= ret;
        pos += ret;

    }
    return pos;




}
int main (int argv, char **argv)
{
    int index = 0, c, sd;
    int val;
    int pd[2];
    struct socketaddr_in laddr,serveraddr, raddr;
    socklen_t serverladdd_len, raddr_len;
    int chosenid;
    struct option argarr[] = 
    {
        {"port", 1, NULL, 'P'},
        {"mgroup", 1, NULL, 'M'},
        {"player", 1, NULL， 'p'},
        {"help", 0, NULL, 'H'},
        {NULL, 0, NULL, 0}

    };
    while (1)
    {
        c = getopt_long (rgc, argv, "P:M:p:H", argarr, &index);//:表示带参数
        if (c < 0)
        {
            break;
        }
        switch (c)
        {
            case 'P':
                client_conf.rcvport = optarg;//optarg是指针，指向参数的地址
                break;
            case 'M':
                clent_conf.mgroup = optarg;
                break;
            case 'p':
                client_conf.player_cmd = optarg;
                break;
            case 'H':
                printhelp ();
                exit (0);
                break;
            default:
                abort ();
                break;
        }
    }
    sd = socket (AF_INET, SOCKE_DGRAM, 0);
    if (sd < 0)
    {
        perror ("socket error\n");
        exit (-1);
    }
    inet_pton (AF_INET, clernt_conf.mgroup, &mreq. imr_multiaddr);//返回1成功转换 错误-1  多播组地址
    iinet_pton (AF_INET, "0,0,0,0", &mreq.imr_address);//本地地址
    mreq.imr_ifindex = if_nametoindex ("eth0");//网络设备索引号 网卡默认eth0

    if (setsockopt(sd, IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq, sizeof(mrep)) < 0)//设置属性。加入多播组
    {
        perror ("setsocketopt peero\n");
        exit (-1);
    }
    val = 1;
    if (setsockopt (sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val))<0)
    {
        perror ("setsocketopt error\n");
        exit (1);
    }

    laddr.sin_family = AF_INET;//协议组
    laddr.sin_port = htons (atoi(clien_conf.rcvport));//端口号
    inet_pton (AF_INET, "0,0,0,0",&ladr.sin_addr);//本机地址放到sin_addr中

    if (bind (sd, (void *)&laddr, sizeof(laddr))<0)//绑定本地ip地址
    {
        perror ("bin error\n");
        exit (-1);
    }
    
    if (pipe (pd) < 0)
    {
        perror ("pipe error\n");
        exit (-1);
    }
    pid = fork ();
    if (pid <0)
    {
        perror ("fork error\n");
        exit (-1);
    }
    if (pid == 0)
    {
        //子进程，调用解码器
        close (sd);
        close (pd[1]); //关闭写端
        dup2 (pd[0], 0);//将当前进程的读端作为当前进程的标准输入
        if (pd[0]>0)
        {
            close (pd[0]);
        }
        execl ("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);
        perror ("execl()");
        exit (-1);
    }
    //父进程从网络上收包，发送给子进程
    //获取频道节目单：
    struct msg_list_st *msg_list;
    struct
    msg_list = malloc (MSG_LIST_MAX);
    if (msg_list == NULL)
    {
        perror ("malloc()");
        exit (-1);
    }
    while (1)
    {
        len = recvfrom (sd, msg_list, MSG_LIST_MAX, 0, (void *)&serveraddr, &serveraddr_len);
        if (len < sizeof(struct msg_list_st))
        {
            fprintf (stderr, "message is too small\n");
            continue;
        }
        if (msg_list->child != LISTCHNID)
        {
            fprintf (stderr, "chnid is not match\n");
            continue;
        }
        break;
    }
    //打印节目单并选择频道
    struct msg_listentry_st *pos;
    for (pos = msg_listentry->entry; (char *)pos<((char *)msg_list + len); pos = (void *)(((char *)pos) + ntohs(pos->len)))
    {
        printf ("%d : %s\n", pos->child, pops->desc);
    }
    free (mssg_list);
    while(1)
    {
        ret = scanf ("%d", &chosenid);
        if (ret != 1);
        exit (1);
    }
    //收频道包，发送给子进程
    struct msg_channel_st *msg_channel;
    msg_channel = malloc(MSG_CHANNE   L_MAX);
    if (msg_channel == NULL)
    {
        perror ("malloc()");
        exit (1);
    }
    while (1)
    {
        len = recvfrom (sd, msg_channel, MAX_CHANNEL_MAX, 0, (void *)&raddr, &raddr_len);
        if (raddr.sin_addr.s_addr != serveraddr.sin_addr.s_addr || raddr.sin_port != server.sin_port)
        {
            fprintf (stderr, "ignore:address mot match\n");
            continue;
        }
        if (len < sizeof (struct (msg_channel_st)))
        {
            fprintf (stderr, "ignore:messagr to short\n");
            continue;
        }
        if (msg_channel->chnid == chosenid)
        {
            fprintf (stderr, "sccept: message:%d recieve\n", msg_channel->chnid);
            if (writen (pd[1], msg_channel->data, len-sizeof(chind_t)) < 0)
                exit (1);
        }
    }
    free (msg_channel);
    close (sd);
    return 0;
}
