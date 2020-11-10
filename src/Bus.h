
//
// Created by matia on 10/11/2020.
//

#ifndef NESEMULATOR_BUS_H
#define NESEMULATOR_BUS_H
#include <cstdint>




class Bus {

public: int8_t RAM[2048] = {0};


public: Bus();

public: ~Bus();

public: int8_t busRead(int16_t address);

public: void busWrite(int16_t address, int8_t data);



};


#endif //NESEMULATOR_BUS_H
