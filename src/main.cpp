#include "Bus.h"
#include "CompactLogger.h"

int main(){

    Bus *bus = new Bus();
    Cartridge *cartridge = new Cartridge("../roms/nestest.nes");
    bus->insertCartridge(*cartridge);
    bus->start();






    return 0;

}