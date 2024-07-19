#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void *doit(void *arg) 
{
    printf ("tid = %lu\t arg = %s\n",pthread_self(), (char *)arg);
    return NULL;
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
    
    sleep (2);
    doit ("main");
    return 0;
}
