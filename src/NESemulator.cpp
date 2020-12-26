//
// Created by olivi on 26.12.2020.
//

#include "NESemulator.h"

NESemulator::NESemulator() :
    cpu(bus),
    ppu(bus, screen)
{
    cartridge = new Cartridge("../roms/donkeyKong.nes");
    bus.insertCartridge(*cartridge);
    masterClock = 0;
    cpu.RESET();
    ppu.setNMI([&](){cpu.NMI();});
}
NESemulator::~NESemulator() {

}

void NESemulator::run() {

    window.create(sf::VideoMode(768,720), "Test Window",
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
                    }
                default:
                    break;
            }
        }
        while(running) {
            if (masterClock%3 == 0) {
                cpu.clock();
            }
            ppu.clock();
            masterClock++;
            if (ppu.frameDone) {
                ppu.frameDone = false;
                break;
            }
        }

        window.clear();
        window.draw(screen);
        window.display();
    }
}