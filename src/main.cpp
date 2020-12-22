#include "Bus.h"
#include "CompactLogger.h"

int main(){
    Cartridge *cartridge = new Cartridge("../roms/donkeyKong.nes");
    Bus *bus = new Bus();
    bus->insertCartridge(*cartridge);
    bus->start();
    return 0;

}