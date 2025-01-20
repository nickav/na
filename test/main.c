#define impl
#include "../na.h"
#include "../na_math.h"
// #include "../na_net.h"

#include <stdlib.h>

int main()
{
    os_init();

    print("Init time: %fs\n", os_time());
    print("Hello, Sailor!\n");
    print("Done! time in ms: %f\n", os_time_in_miliseconds());
    
    return 0;
}