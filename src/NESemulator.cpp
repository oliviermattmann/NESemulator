//
// Created by olivi on 26.12.2020.
//

#include "NESemulator.h"

NESemulator::NESemulator() //:
    //cpu(bus),
    //ppu(bus, screen)
{
    bus = new Bus();
    cartridge = new Cartridge("../roms/nestest.nes");
    bus->insertCartridge(*cartridge);
    cpu = new CPU6502(bus);
    ppu = new PPU2C02(bus, screen);
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
    screen.init(256, 240, 3, sf::Color::Cyan);
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
                window.draw(screen);
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
        for (int i = 0; i < 8; i++) {
            window.draw(cpuStatus[i]);
        }
        window.draw(screen);
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