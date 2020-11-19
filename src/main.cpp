#include "Bus.h"
#include "CPU6502.h"
#include "tests.h"
#include <cstdio>
#include "CompactLogger.h"

int main(){

    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502(bus);

    cpu->write(0x0, 0x1);
    cpu->write(0x3, 0xFF);
    cpu->write(0x6, 0xFF);

    cpu->displayMemoryPage(0);
    cpu->displayMemoryPage(1);

    /*
    cpu->testInstruction(0x29);
     */

    return 0;

}