#include <iostream>
#include <cstring>

int main ()
{
    unsigned char cirmsg[] = {0x10, 0x02, 0x00, 0x13, 0x00, 0x01, 0x00, 0x01, 0xe5, 0x20, 0x20, 0x01, 0x03, 0x10, 0x10, 0x09, 0x00, 0x00, 0xf5, 0x00, 0x1e, 0x0b, 0x31, 0x10, 0x03};
    int  i = 0, j = 0;
    int size = sizeof(cirmsg)/sizeof(cirmsg[0]);
    for (i = 0; i<size; i++)
    {
        if (cirmsg[i] == 0x10)
        {
            if (i+1<size && cirmsg[i+1] == 0x10)
            {
                i+=2;
                printf ("[%d] = %d\n", i, cirmsg[i]);
            }
            else
            {
                printf ("[%d] = %d\n", i, cirmsg[i]);
            }
        }
        else
            printf ("[%d] = %d\n", i, cirmsg[i]);
    }
    return 0;
}
