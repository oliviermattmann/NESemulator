//
// Created by matia on 10/11/2020.
//
#include "Bus.h"
#include "Cartridge.h"

#include <utility>

Bus::Bus(){
    cartridge = Cartridge();
    for(uint8_t & text : RAM) {
        text = 0x00;
    }
}
Bus::~Bus() {
    //does nothing
}

uint8_t Bus::busRead(uint16_t address){
    if(address <= 0x1FFF) {
        return RAM[address % 0x07FF];
    } else if (address >= 0x4020 & address <= 0xFFFF) {
        return cartridge.prgData[(address - 0x4020) % cartridge.BANKSIZE];
    }
    return -1;
}
void Bus::busWrite(uint16_t address, uint8_t data){
    RAM[address] = data;
}

void Bus::insertCartridge(Cartridge cartridge) {
    this->cartridge = std::move(cartridge);
}



