//
// Created by Startklar on 07.12.2020.
//

#ifndef NESEMULATOR_NESTESTPARSER_H
#define NESEMULATOR_NESTESTPARSER_H
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>


class NesTestParser {
public:
    std::ifstream logFile;
    uint16_t lineArray[8];
    std::string line;

    NesTestParser();
    ~NesTestParser();


    void getLine();
    void validate(uint16_t pc, uint8_t op, uint16_t address, uint8_t acc, uint8_t x, uint8_t y, uint8_t sr, uint16_t sp);



};


#endif //NESEMULATOR_NESTESTPARSER_H
