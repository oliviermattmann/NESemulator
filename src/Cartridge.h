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

/**
 * Represents the game file in an accessible way for the emulator.
 */
class Cartridge {
public:
    std::vector<uint8_t> prgData;
    std::vector<uint8_t> chrData;

    Mapper mapper; //Mapper used by the cartridge, not used in our emulator
    uint8_t nBanksPrg; //Number of banks of PRG ROM
    uint8_t nBanksChr; //Number of banks of CHR ROM
    uint8_t mapper_id; //Number of the mapper
    uint8_t mirroring; //0 = horizontal (CIRAM A10 = PPU A11) | 1 = vertical (CIRAM A10 = PPU A10)
    uint8_t trainer;   //1 if trainer present, (in our case no trainer)

    unsigned int BANKSIZE = 16384; //Size of a program data bank (Half for PPU-data)

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
