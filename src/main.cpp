#include "Bus.h"
#include "CPU6502.h"
#include <cstdio>

int main(){
    Bus *bus = new Bus();
    bus->busWrite(0x0000, 0);
    CPU6502 cpu = new CPU6502();
    cpu.run(bus);
    return 0;

}