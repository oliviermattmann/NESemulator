#include "Bus.h"
#include "CPU6502.h"
#include "tests.h"
#include <cstdio>
#include "CompactLogger.h"

int main(){

    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502(bus);


    cpu->testInstruction(0x29);

    return 0;

}