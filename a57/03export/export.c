#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE ("GPL");
int add (int x, int y)
{
    return x+y;
}
EXPORT_SYMBOL (add);
