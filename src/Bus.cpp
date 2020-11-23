//
// Created by matia on 10/11/2020.
//
#include "Bus.h"

Bus::Bus(){
    for(uint8_t & text : RAM)
        text = 0x00;
}
Bus::~Bus() {
    //does nothing
}

uint8_t Bus::busRead(uint16_t address){
    return RAM[address];
}
void Bus::busWrite(uint16_t address, uint8_t data){
    RAM[address] = data;
}



