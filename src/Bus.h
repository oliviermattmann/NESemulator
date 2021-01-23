
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


//forward declarations
class Cartridge;
class PPU2C02;
class CPU6502;

class Bus {

public:
    uint8_t RAM[2048];
    Cartridge cartridge;
    //controller where NESEmulator writes to
    uint8_t controller;
    //controller the cpu reads from
    uint8_t controller_status;

    PPU2C02 *ppu = nullptr;
    CPU6502 *cpu = nullptr;

    //Both the ppu and nesemulator have access to this class, so the dma variables are the here
    bool dma = false;
    bool idle = true;

    uint8_t dmaData;
    uint8_t dmaPage;
    uint8_t dmaAddress;

    Bus();

    ~Bus();

    //Reads
    uint8_t busRead(uint16_t address);
    //writes
    void busWrite(uint16_t address, uint8_t data);
    //initialized the cartridge
    void insertCartridge(Cartridge cartridge);
};
#endif //NESEMULATOR_BUS_H
