
//
// Created by matia on 10/11/2020.
//

#ifndef NESEMULATOR_BUS_H
#define NESEMULATOR_BUS_H
#include <cstdint>
#include "Cartridge.h"
#include "SFML/System.hpp"


class Cartridge;



class Bus {

public:
    uint8_t RAM[2048];
    uint8_t masterClock;
    Cartridge cartridge;



    Bus();

    ~Bus();

    uint8_t busRead(uint16_t address);

    void busWrite(uint16_t address, uint8_t data);

    void insertCartridge(Cartridge cartridge);

    void busClock();

    //void start();


};


#endif //NESEMULATOR_BUS_H
