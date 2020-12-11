#include "Bus.h"
#include "CompactLogger.h"

int main(){
    printf("%d\n", (10 % 3));
    printf("%d\n", (10 & 3));

    Cartridge *cartridge = new Cartridge("../roms/donkeyKong.nes");
    Bus *bus = new Bus();
    bus->insertCartridge(*cartridge);
    bus->start();
    return 0;
}