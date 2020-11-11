#include "Bus.h"
#include "CPU6502.h"
#include <cstdio>

int main(){
    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502();
    bus->busWrite(0x0000, 0x00);
    bus->busWrite(0x0001, 0x10);
    cpu->run(*bus);
    return 0;

}