#include "Bus.h"
#include <cstdio>

int main(){
    Bus *bus = new Bus();
    

    bus->busWrite(0x0000, 100);
    printf("%d", bus->busRead(0x0000));
    return 0;
}