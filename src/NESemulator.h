//
// Created by olivi on 26.12.2020.
//

#ifndef NESEMULATOR_NESEMULATOR_H
#define NESEMULATOR_NESEMULATOR_H
#include "../util/CompactLogger.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"

#include "CPU6502.h"
#include "PPU2C02.h"
#include "Bus.h"
#include "Cartridge.h"


class NESemulator {
public:
    Cartridge* cartridge;
    //Bus bus;
    Bus *bus;
    CPU6502 *cpu;
    PPU2C02 *ppu;
    uint16_t masterClock;

    sf::RenderWindow window;
    sf::Text cpuStatus[8];
    sf::Font myFont;
    Screen mainScreen;
    Screen patternScreens[2];

    bool instructionStep = false;
    bool frameStep = false;

    //Constructor/Destructor
    NESemulator();
    ~NESemulator();
    //functions
    void run();
    void initStatus();
    void updateStatus();
    void clock();
    void drawPatternTable(int number);


};


#endif //NESEMULATOR_NESEMULATOR_H
