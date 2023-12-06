#include <iostream>
using namespace std;
int main ()
{
    int x = 2,y = 0, z = 1;
    x*=(y=z=3); cout << x << endl;
    z = 5; 
    x==(y=z); cout << x << endl;
    x=(y==z); cout << x << endl;
    x=(y&z); cout << x << endl;
    x=(y&&z); cout << x << endl;
    y=6;
    x= (y|z); cout << x << endl;
    x= (y||z); cout << x << endl;
    z= 0xfffffefe;
    x= ~z<<3;  cout << x << endl;
    return 0;
}
