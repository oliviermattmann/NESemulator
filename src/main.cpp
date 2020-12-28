#include "Bus.h"
#include "NESemulator.h"


int main(){
    printf("%d\n", (10 % 3));
    printf("%d\n", (10 & 3));

    /*Cartridge *cartridge = new Cartridge("../roms/donkeyKong.nes");
    //initialize Components
    Screen screen = Screen(768, 720, 3, sf::Color::Green);
    Bus bus = Bus(screen);
    bus.insertCartridge(*cartridge);
    bus.cpu6502 = CPU6502();
    bus.connectCPU();
    bus.ppu2C02 = PPU2C02(bus, screen);
    bus.connectPPU(screen);
    bus.cpu6502.RESET();*/

    //initialize Screen related stuff
    NESemulator neSemulator;
    neSemulator.run();


    return 0;

}