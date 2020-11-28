//
// Created by Startklar on 28.11.2020.
//

#include "PPU2C02.h"

PPU2C02::PPU2C02() {

}

PPU2C02::~PPU2C02() {

}

uint8_t PPU2C02::readPPU(uint16_t adress) {

}

void PPU2C02::writePPU(uint16_t adress, uint8_t data) {


}

uint8_t PPU2C02::readCPU(uint16_t address) {
    uint8_t data = 0x00;
    switch(address & 0x0007) {          //basically looking at the last 3 bits, because of mirroring
        case(0x0000):                   //PPUCTRL
            break;
        case(0x0001):                   //PPUMASK
            break;
        case(0x0002):                   //PPUSTATUS
            break;
        case(0x0003):                   //OAMADDR
            break;
        case(0x0004):                   //OAMDATA
            break;
        case(0x0005):                   //PPUSCROLL
            break;
        case(0x0006):                   //PPUADDR
            break;
        case(0x0007):                   //PPUDATA
            break;
    }

}

void PPU2C02::writeCPU(uint16_t address, uint8_t data) {

    switch(address & 0x0007) {
        case(0x0000):                   //PPUCTRL
            break;
        case(0x0001):                   //PPUMASK
            break;
        case(0x0002):                   //PPUSTATUS
            break;
        case(0x0003):                   //OAMADDR
            break;
        case(0x0004):                   //OAMDATA
            break;
        case(0x0005):                   //PPUSCROLL
            break;
        case(0x0006):                   //PPUADDR
            break;
        case(0x0007):                   //PPUDATA
            break;
    }

}