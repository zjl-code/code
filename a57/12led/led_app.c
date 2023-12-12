#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char** argv )
{
    char buf[2], value;
    if (argc !=3 || atoi(argv [2])!=0 && atoi(argv[2])!=1) 
    {
        printf ("usage:\n");
        printf ("  ./led_app /dec/led_gpio5_0 <val\n>");
        printf ( " <val> :0/1\n");
        exit (0);
    }
    int fd = 0;
    fd = open (argv[1], O_RDWR);
    if (fd < 0)
    {
        perror ("open failed\n");
        exit (-1);
    }
   int ret = read (fd, &buf[0], 1);
    if (ret >=0)
    {
        printf ("read gpio5_0 value:%d\n", buf[0]);

    }
    else
    {
        perror ("read failed\n");
        exit (-1);
    }
    
    value = atoi(argv[2]);
    ret = write (fd, &value, 1);
    if (ret = -1)
    {
        perror ("write failed\n");
        close (fd);
    }
    close (fd);



   return 0;
}
