
//
// Created by matia on 10/11/2020.
//

#ifndef NESEMULATOR_BUS_H
#define NESEMULATOR_BUS_H
#include <cstdint>
#include "Cartridge.h"
#include "CPU6502.h"
#include "PPU2C02.h"
#include "Renderer.h"
#include "SFML/System.hpp"


class Cartridge;



class Bus {

public:
    uint8_t RAM[2048];
    uint8_t masterClock;
    Cartridge cartridge;
    CPU6502 cpu6502;
    PPU2C02 ppu2C02;
    Renderer* renderer;
    sf::Thread* thread;



    Bus();

    ~Bus();

    uint8_t busRead(uint16_t address);

    void busWrite(uint16_t address, uint8_t data);

    void insertCartridge(Cartridge cartridge);

    void busClock();

    void start();

    void connectCPU();

    void connectPPU();
};


#endif //NESEMULATOR_BUS_H
