//
// Created by matia on 10/11/2020.
//
#include "Bus.h"

Bus::Bus(){
    //Empty
}

int8_t Bus::busRead(int16_t address){
    return RAM[address];
}
void Bus::busWrite(int16_t address, int8_t data){
    RAM[address] = data;
}



