#include "Bus.h"
#include "CPU6502.h"
#include "Cartridge.h"
#include "tests.h"
#include <cstdio>
#include "CompactLogger.h"

int main(){

    Bus *bus = new Bus();
    CPU6502 *cpu = new CPU6502(bus);
    Cartridge *cartridge = new Cartridge("../roms/DonkeyKong.nes");
    bus->insertCartridge(*cartridge);
    std::cout << std::endl << hex << (int)cpu->read(0x6000);
    cpu->run();






    return 0;

}