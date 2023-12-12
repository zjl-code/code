#include <iostream>
#include <cstring>
using namespace std;
int main ()
{
    char machineNum1[14] = {0x30, 0x10, 0x00, 0x11};
    char machineNum2[14] = {0x30, 0x10, 0x00, 0x22};
    int a = strcmp (machineNum1, machineNum2);
    int b = memcmp (machineNum1, machineNum2, 14);
    cout << "a = " << a << endl;
    cout << "b=" << b << endl;

    return 0;
}


