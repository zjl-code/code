#ifndef __PROTOL_H__
#define __PROTOL_H__
#include <site_type.h>
#define DEFAULT_MGROUP  “224.2.2.2”//多播地址
#define DEFAULT_RCVPORT "1989"
#define CHNNR 100
#define LISTCHNNID 0
#define MINCHNNID 1
#define MAXCHNNID (MINCHNNID+CHNNR-1)
#define MSG_CHANNEL_MAX (65536-20-8)//结构体大小  //udp包的推荐长度是512～64K
                                              //ip包头20-60字节，udp报头8
#define MAX_DATA (MSG_CHANNEL_MAX-sizeof(chnid_t))  //在结构体中数据的最大
struct msg_channel_st
{
    chnid_t chnid;//存储范围(0~255)  chnid范围MINCHNNID-MAXCHNNID
    uint8_t data[1];//变长数组
}__attribute__((packed));//非对齐，udp在个各平台上对齐方式不一样
struct msg_listentry_st//存放描述信息
{
    chnid_t chnid;//频道号
    uint8_t desc[1];//变长
}__attribute__((pached));
struct msg_list_st
{
    chnid_t chnid;//必须是0
    struct msg_listentry_st entry[1];//描述信息结构体数组
}__attribute__((packed));

#endif
