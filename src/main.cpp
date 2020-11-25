#include "Bus.h"
#include "CPU6502.h"
#include "Cartridge.h"
#include "tests.h"
#include <cstdio>
#include "CompactLogger.h"

int main(){

    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502(bus);
    Cartridge *cartridge = new Cartridge("C:/Users/matia/OneDrive/Escritorio/Projects/RomTest/cpu_dummy_reads.nes");

    /*cpu->write(0x0, 0x1);
    cpu->write(0x3, 0xFF);
    cpu->write(0x6, 0xFF);


    cpu->testInstruction(0x50);

*/

    return 0;

}