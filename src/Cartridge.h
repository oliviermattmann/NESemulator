//
// Created by matia on 25/11/2020.
//

#ifndef NESEMULATOR_CARTRIDGE_H
#define NESEMULATOR_CARTRIDGE_H
#include <cstdint>
#include<iostream>

#include<vector>
using namespace std;

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
    uint8_t mirroring;
    uint8_t trainer;

    unsigned int BANKSIZE = 16384;

    Cartridge();

    Cartridge(const std::string& fileName);
    void getHeader(vector<uint8_t> content);
};

enum BitMask {
    FIRST = 1 << 0,
    SECOND = 1 << 1,
    THIRD = 1 << 2,
    FOURTH = 1 << 3,
    FIFTH = 1 << 4,
    SIXTH = 1 << 5,
    SEVENTH = 1 << 6,
    EIGHTH = 1 << 7
};


#endif //NESEMULATOR_CARTRIDGE_H
