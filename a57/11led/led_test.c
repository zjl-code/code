#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (void)
{
    int fd = open ("/dev/led_gpio5_0", O_RDWR);
    if (fd  < 0)
    {
        perror ("open failed");
        return -1;
    }
    printf ("open successed!\n");
    sleep (10);
    printf ("close fi:le \n");
    close (fd);
    return 0;
}
