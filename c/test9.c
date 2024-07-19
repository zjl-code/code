#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
void *doit(void *arg) 
{
    printf ("arg = %s\n", (char *)arg);
    return NULL;
    //    return (void *)-1;
}
int main (void)
{
    pthread_t tid;
    int a = pthread_create (&tid, NULL, doit, "new");
    if (a )
    {
        perror ("pthread_create");
        return -1;
    }
    
    //不休眠不确定会打印一个还是两个跟系统的调度有关
    //确保新线程先执行
   // sleep (2);
    //验证了一个进程有多个线程
    doit ("main");
    return 0;
}
