//
// Created by olivi on 26.12.2020.
//

#include "NESemulator.h"

NESemulator::NESemulator() //:
    //cpu(bus),
    //ppu(bus, screen)
{
    bus = new Bus();
    cartridge = new Cartridge("../roms/DonkeyKong.nes");
    bus->insertCartridge(*cartridge);
    cpu = new CPU6502(bus);
    ppu = new PPU2C02(bus, mainScreen);
    masterClock = 0;
    cpu->RESET();
    ppu->setNMI([&](){cpu->NMI();});

    initStatus();
}
NESemulator::~NESemulator() {

}

void NESemulator::run() {

    window.create(sf::VideoMode(1024, 960), "Test Window",
                  sf::Style::Titlebar | sf::Style::Close);
    mainScreen.init(128, 120, 5, 50, 50, sf::Color::Cyan);
    patternScreens[0].init(128, 128, 2, 750 ,100, sf::Color::White);
    patternScreens[1].init(128, 128, 2, 750, 400, sf::Color::Green);
    sf::Event event;
    bool running = false;


    while(window.isOpen()) {
        while(window.pollEvent(event)) {

            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    exit(0);
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                        exit(0);
                    } else if (event.key.code == sf::Keyboard::P) {
                        running = !running;
                    } else if (event.key.code == sf::Keyboard::Y) {
                        bus->controller |= 0x80;
                    } else if (event.key.code == sf::Keyboard::X) {
                        bus->controller |= 0x40;
                    } else if (event.key.code == sf::Keyboard::A) {
                        bus->controller |= 0x10;
                    } else if (event.key.code == sf::Keyboard::S) {
                        bus->controller |= 0x20;
                    } else if (event.key.code == sf::Keyboard::Up) {
                        bus->controller |= 0x08;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        bus->controller |= 0x04;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        bus->controller |= 0x01;
                    } else if (event.key.code == sf::Keyboard::Left) {
                        bus->controller |= 0x02;
                    } else if (event.key.code == sf::Keyboard::I) {
                        if (!running) {
                            do {clock();} while (!cpu->instructionComplete);
                            updateStatus();
                            cpu->instructionComplete = false;
                        }
                    } else if (event.key.code == sf::Keyboard::O) {
                        frameStep = !frameStep;
                    }
                    break;
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Y) {
                        bus->controller ^= 0x80;
                    } else if (event.key.code == sf::Keyboard::X) {
                        bus->controller ^= 0x40;
                    } else if (event.key.code == sf::Keyboard::A) {
                        bus->controller ^= 0x10;
                    } else if (event.key.code == sf::Keyboard::S) {
                        bus->controller ^= 0x20;
                    } else if (event.key.code == sf::Keyboard::Up) {
                        bus->controller ^= 0x08;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        bus->controller ^= 0x04;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        bus->controller ^= 0x01;
                    } else if (event.key.code == sf::Keyboard::Left) {
                        bus->controller ^= 0x02;
                    }
                    break;

                default:
                    break;
            }
        }
        //window.clear();
        if (running) {
            while (!ppu->frameDone) {
                clock();
                updateStatus();
                window.clear();
                window.draw(mainScreen);
                for (int i = 0; i < 8; i++) {
                    window.draw(cpuStatus[i]);
                }
                window.display();
            }
            ppu->frameDone = false;
        } else {
            /*if (instructionStep) {
                //while (!cpu.instructionComplete) {
                  //  clock();
                //}
                do {clock();} while (!cpu->instructionComplete);
                instructionStep = false;
                updateStatus();
            } else*/ if (frameStep) {

                do {clock();} while (!ppu->frameDone);
                frameStep = false;
                ppu->frameDone = false;
            }
        }
        window.clear();
        mainScreen.setPixel(1, 90, sf::Color::Red);
        ppu->drawToScreen();
        for (int i = 0; i < 8; i++) {
            window.draw(cpuStatus[i]);
        }
        drawPatternTable(0);
        drawPatternTable(1);
        window.draw(mainScreen);
        window.draw(patternScreens[0]);
        window.draw(patternScreens[1]);
        window.display();
    }
}
void NESemulator::initStatus() {
    if(!myFont.loadFromFile("../external/Fonts/ARIAL.TTF")) {
        std::cout << "fuck" << std::endl;
    }
    for (int i = 0; i < 8; i++) {
        cpuStatus[i].setFont(myFont);
        if (i == 2) {
            cpuStatus[i].setPosition(775+i*30+7, 0);
        } else {
            cpuStatus[i].setPosition(775+i*30, 0);
        }

        cpuStatus[i].setCharacterSize(30);
        cpuStatus[i].setStyle(sf::Text::Bold);
        cpuStatus[i].setFillColor(sf::Color::Green);
    }
    cpuStatus[0].setString("C");
    cpuStatus[1].setString("Z");
    cpuStatus[2].setString("I");
    cpuStatus[3].setString("D");
    cpuStatus[4].setString("B");
    cpuStatus[5].setString("U");
    cpuStatus[6].setString("V");
    cpuStatus[7].setString("N");
}
void NESemulator::updateStatus() {
    for (int i = 0; i < 8; i++) {
        if (cpu->SR & 1<<i) {
            cpuStatus[i].setFillColor(sf::Color::Green);
        } else {
            cpuStatus[i].setFillColor(sf::Color::Red);
        }
    }
}

void NESemulator::clock() {
    ppu->clock();
    if (masterClock%3 == 0) {
        cpu->clock();
    }
    masterClock++;
}

//Print pattern Table atm
void NESemulator::drawPatternTable(int number) {
    uint8_t patternTableNumber = number;
    uint8_t x = 0, y = 0;
    sf::Color col;
    for (int i = 0; i < 256; i++) {
        if (i >= 256) {
            i = 1;
        }
        ppu->getPatternTile(i + number * 256);
        x = (i* 8)% 128;
        if(i%16 == 0 && i !=0) {
            y +=8;
        }
        for(int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                switch (ppu->pixelData[j][k]) {
                    case 0: col = ppu->getColor(0x0f);
                        break;
                    case 1: col = ppu->getColor(0x15);
                        break;
                    case 2: col = ppu->getColor(0x2c);
                        break;
                    case 3: col = ppu->getColor(0x12);
                        break;
                }
                int xVal = x + k;
                int yVal = y + j;

                patternScreens[patternTableNumber].setPixel(xVal, yVal, col);
            }
        }
    }
}