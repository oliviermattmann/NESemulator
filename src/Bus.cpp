//
// Created by matia on 10/11/2020.
//
#include "Bus.h"

Bus::Bus(){
    for(int8_t &text : RAM)
        text = 0x00;
}
Bus::~Bus() {
    //does nothing
}

int8_t Bus::busRead(uint16_t address){
    return RAM[address];
}
void Bus::busWrite(uint16_t address, int8_t data){
    RAM[address] = data;
}



