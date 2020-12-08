//
// Created by Startklar on 07.12.2020.
//

#include "NesTestParser.h"

NesTestParser::NesTestParser() {
    logFile.open("../roms/nestest.log");

}
NesTestParser::~NesTestParser(){}

void NesTestParser::getLine() {
    if (logFile.is_open()) {
        std::getline(logFile, line);
        lineArray[0] = (uint16_t)stol(line.substr(0,4), nullptr, 16);
        lineArray[1] = (uint16_t)stol(line.substr(6,2), nullptr, 16);
        std::string addr = line.substr(20, 6);
        if (addr.front() == '#' || addr.front() == ' ' || addr.front() == 'A' || addr.front() == '(') {
            lineArray[2] = NULL;
        } else if (addr.back() == ',') {
            lineArray[2] = (uint16_t)stol(line.substr(30, 4), nullptr, 16);
        } else if (addr.at(3) == ',') {
            lineArray[2] = (uint16_t)stol(line.substr(28, 2), nullptr, 16);
        }else  {
            lineArray[2] = (uint16_t)stol(line.substr(21, 4), nullptr, 16);
        }
        std::size_t t = line.find("A:");
        lineArray[3] = (uint16_t)stol(line.substr(t+2, 2), nullptr, 16);
        t = line.find("X:");
        lineArray[4] = (uint16_t)stol(line.substr(t+2, 2), nullptr, 16);
        t = line.find("Y:");
        lineArray[5] = (uint16_t)stol(line.substr(t+2, 2), nullptr, 16);
        t = line.find("P:");
        lineArray[6] = (uint16_t)stol(line.substr(t+2, 2), nullptr, 16);
        t = line.find("SP:");
        lineArray[7] = (uint16_t)stol(line.substr(t+3, 2), nullptr, 16);
    }
}

void NesTestParser::validate(uint16_t pc, uint8_t op, uint16_t address, uint8_t acc, uint8_t x, uint8_t y, uint8_t sr, uint16_t sp) {
    if (lineArray[0] != pc) {
        std::cout << "wrong PC, was: " << std::setfill('0') << std::setw(4)<< std::hex << pc << " should have been: " << std::setfill('0') << std::setw(4)<< std::hex << lineArray[0] << std::endl;

        std::cout << "filler" << std::endl;
    }
    if (lineArray[1] != op) {
        std::cout << "wrong op_code, was: " << std::setfill('0') << std::setw(2)<< std::hex << op << " should have been: " << std::setfill('0') << std::setw(2)<< std::hex << lineArray[1] << std::endl;

        std::cout << "filler" << std::endl;
    }
    if (lineArray[2] != NULL && lineArray[2] != address) {
        std::cout << "wrong addressParam, was: " << std::setfill('0') << std::setw(2)<< std::hex << address << " should have been: " << std::setfill('0') << std::setw(2)<< std::hex << lineArray[2] << std::endl;

        std::cout << "filler" << std::endl;
    }
    if (lineArray[3] != acc) {
        std::cout << "wrong accumulator, was: " << std::setfill('0') << std::setw(2)<< std::hex << (int)acc << " should have been: " << std::setfill('0') << std::setw(2)<< std::hex << lineArray[3] << std::endl;

        std::cout << "filler" << std::endl;
    }
    if (lineArray[4] != x) {
        std::cout << "wrong x register, was: " << std::setfill('0') << std::setw(2)<< std::hex << x << " should have been: " << std::setfill('0') << std::setw(2)<< std::hex << lineArray[4] << std::endl;

        std::cout << "filler" << std::endl;
    }
    if (lineArray[5] != y) {
        std::cout << "wrong y register, was: " << std::setfill('0') << std::setw(2)<< std::hex << y << " should have been: " << std::setfill('0') << std::setw(2)<< std::hex << lineArray[5] << std::endl;

        std::cout << "filler" << std::endl;
    }
    if (lineArray[6] != sr) {
        std::bitset<8> actual(sr);
        uint8_t expec = (uint8_t)lineArray[6];
        std::bitset<8> expected(expec);
        std::cout << "wrong Status register, was: " << actual << " ,should have been: " << expected << std::endl;

        std::cout << "filler" << std::endl;
    }
    if (lineArray[7] != sp) {
        std::cout << "wrong SP, was: " << std::setfill('0') << std::setw(2) << std::hex << sp << " should have been: " << std::setfill('0') << std::setw(2)<< std::hex << lineArray[7] << std::endl;

        std::cout << "filler" << std::endl;
    }

}


