
//
// Created by matia on 10/11/2020.
//

#ifndef NESEMULATOR_BUS_H
#define NESEMULATOR_BUS_H
#include <cstdint>
#include "Cartridge.h"
#include "SFML/System.hpp"
#include "PPU2C02.h"
#include "CPU6502.h"


class Cartridge;
class PPU2C02;
class CPU6502;


class Bus {

public:
    uint8_t RAM[2048];
    uint8_t masterClock;
    Cartridge cartridge;
    uint8_t controller;
    uint8_t controller_status;
    PPU2C02 *ppu = nullptr;
    CPU6502 *cpu = nullptr;



    Bus();

    ~Bus();

    uint8_t busRead(uint16_t address);

    void busWrite(uint16_t address, uint8_t data);

    void insertCartridge(Cartridge cartridge);

    void busClock();

    //void start();


};


#endif //NESEMULATOR_BUS_H
