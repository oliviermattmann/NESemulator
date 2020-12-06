#include "Bus.h"
#include "CompactLogger.h"

int main(){

    Bus *bus = new Bus();
    Cartridge *cartridge = new Cartridge("../roms/DonkeyKong");
    bus->insertCartridge(*cartridge);
    bus->start();






    return 0;

}