//
// Created by matia on 10/11/2020.
//
#include "Bus.h"


Bus::Bus() {
    cartridge = Cartridge();
    controller = 0x00;
    controller_status = 0x00;

    for(uint8_t & text : RAM) {
        text = 0x00;
    }
}
Bus::~Bus() {
    //does nothing
}

uint8_t Bus::busRead(uint16_t address){
    //reading from main memory
    if(address <= 0x1FFF) {
        return RAM[address & 0x07FF];
    }
    //read from ppu registers
    else if (address >= 0x2000 & address <= 0x3FFF) {
        return  ppu->readCPU(address);
    }
    //Address range for the cartridge data
    else if (address >= 0x8000 & address <= 0xFFFF) {
        return cartridge.prgData[address&(cartridge.nBanksPrg > 1 ? 0x7FFF:0x3FFF)];
    }
    //Controller
    else if (address >= 0x4016 && address <= 0x4017)
    {
        //Read out the MSB of the controller status word
        //read 8 times in a row to get all the data
        uint8_t bit = (controller_status & 0x80) > 0;
        controller_status <<= 1;
        return bit;
    }
    return -1;
}
void Bus::busWrite(uint16_t address, uint8_t data){
    //writes to the main memory


    if (address <= 0x1FFF) {
        //address is mirrored because the memory is is only 2KB


        RAM[address & 0x07FF] = data;
    }
    //write to this address transfers the controller byte buffered into the status for the cpu to be read 1 bit at a time
    else if (address >= 0x4016 && address <= 0x4017) {
        //currently only one controller
        controller_status = controller;
    }
    //A write to this address range goes to the ppu, the address further processed in the ppu
    else if (address >= 0x2000 & address <= 0x3FFF) {
        ppu->writeCPU(address, data);
    }
    //a write to this address signals an incoming dma transfer
    else if (address == 0x4014) {
        ppu->writeCPU(address, data);
    }
}

void Bus::insertCartridge(Cartridge cartridge) {
    this->cartridge = std::move(cartridge);
}



