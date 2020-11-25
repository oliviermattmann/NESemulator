//
// Created by matia on 25/11/2020.
//

#ifndef NESEMULATOR_CARTRIDGE_H
#define NESEMULATOR_CARTRIDGE_H
#include <cstdint>
#include<iostream>

#include<vector>
using namespace std;

#include "Bus.h"
#include "Mapper.h"
#include <fstream>
class Cartridge {
public:
    std::vector<uint8_t> prgData;
    std::vector<uint8_t> chrData;

    Mapper mapper;
    uint8_t nBanksPrg;
    uint8_t nBanksChr;
    uint8_t mapper_id;



    Cartridge(const std::string& fileName);
    void getHeader(const std::string& fileName);
};


#endif //NESEMULATOR_CARTRIDGE_H
