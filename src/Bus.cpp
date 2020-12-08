//
// Created by matia on 10/11/2020.
//
#include "Bus.h"

Bus::Bus(){
    cartridge = Cartridge();
    masterClock = 0;
    for(uint8_t & text : RAM) {
        text = 0x00;
    }
    connectCPU();
    connectPPU();

}
Bus::~Bus() {
    //does nothing
}

uint8_t Bus::busRead(uint16_t address){
    if(address <= 0x1FFF) {
        return RAM[address & 0x07FF];
    } else if (address >= 0x4020 & address <= 0xFFFF) {
        return cartridge.prgData[address%cartridge.BANKSIZE];//(address - 0x4020) % cartridge.BANKSIZE];
        //for nestest.nes this does not work us address%cartridge.BANKSIZE as index instead
    }
    return -1;
}
void Bus::busWrite(uint16_t address, uint8_t data){
    if (address <= 0x1FFF) {
        RAM[address & 0x07FF] = data;
    }

}

void Bus::insertCartridge(Cartridge cartridge) {
    this->cartridge = std::move(cartridge);
}

void Bus::start() {
    while(1) {
        busClock();
    }
}
void Bus::connectCPU() {
    cpu6502.bindToBus(this);
}
void Bus::connectPPU() {
    ppu2C02.bindToBus(this);
}
void Bus::busClock() {
    if(masterClock % 3 == 0) {
        cpu6502.clock();
    }
    ppu2C02.clock();
    masterClock++;
}



