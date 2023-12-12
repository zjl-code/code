#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char** argv )
{
    char buf[2];
    int status = 0;
    char cmd;
    if (argc !=2 ) 
    {
        printf (" usage:%s <on/off>", argv[0]);
        exit (0);
    }
    int fd = 0;
    fd = open ("/dev/led_gpio5_0", O_RDWR);
    if (fd < 0)
    {
        perror ("open failed\n");
        exit (-1);
    }
    if (strcmp(argv[1], "on")==0)
    {
        cmd = 1;
    }
    else if (strcmp(argv[1],"off")==0)
    {
        cmd = 0;
    }
    write (fd, &cmd,sizeof (char));
    read (fd, &status, sizeof (char));

    if (status == 0)
    {
        printf ("led id off\n");
    }
        close (fd);
    close (fd);



   return 0;
}
