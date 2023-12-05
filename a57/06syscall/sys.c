#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
int main (void)
{
    int ret = 0;
    ret = syscall (400, 10, 20);
    printf ("ret = %d\n", ret);
    return 0;
}
