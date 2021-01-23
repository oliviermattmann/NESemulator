//
// Created by olivi on 26.12.2020.
//

#include "NESemulator.h"

NESemulator::NESemulator() {
    bus = new Bus();
    cartridge = new Cartridge("../roms/SMB.nes");
    bus->insertCartridge(*cartridge);
    cpu = new CPU6502(bus);
    ppu = new PPU2C02(bus, mainScreen);
    masterClock = 0;
    cpu->RESET();
    ppu->setNMI([&](){cpu->NMI();});

    initStatus();
    initControllerInfo();
}

NESemulator::~NESemulator() {}

void NESemulator::run() {
    sf::Event event;
    //Initialize the different screens
    window.create(sf::VideoMode(1024, 960), "NES Emulator",
                  sf::Style::Titlebar | sf::Style::Close);
    mainScreen.init(256, 240, 2.5, 50, 50, sf::Color::Black);
    patternScreens[0].init(128, 128, 2, 750 ,100, sf::Color::White);
    patternScreens[1].init(128, 128, 2, 750, 400, sf::Color::Green);

    //set Framerate Limit
    window.setFramerateLimit(60);

    //we can already prepare the patterntables as they don't change
    drawPatternTable(0);
    drawPatternTable(1);

    while(window.isOpen()) {
        //Event Poll for keyboard input
        while(window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    exit(0);
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                        exit(0);
                        //Set the appropriate Bit in the controller byte for the cpu to read.
                    } else if (event.key.code == sf::Keyboard::Y) {     //A Button
                        bus->controller |= 0x80;
                    } else if (event.key.code == sf::Keyboard::X) {     //B Button
                        bus->controller |= 0x40;
                    } else if (event.key.code == sf::Keyboard::A) {     //Select Button
                        bus->controller |= 0x10;
                    } else if (event.key.code == sf::Keyboard::S) {     //Start Button
                        bus->controller |= 0x20;
                    } else if (event.key.code == sf::Keyboard::Up) {
                        bus->controller |= 0x08;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        bus->controller |= 0x04;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        bus->controller |= 0x01;
                    } else if (event.key.code == sf::Keyboard::Left) {
                        bus->controller |= 0x02;
                    } else if (event.key.code == sf::Keyboard::R) {
                        cpu->RESET();
                    }
                    break;
                case sf::Event::KeyReleased:
                    //Once the Key is released again the appropriate bit is cleared again
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
            //we clock as many times as we need for the ppu to finish drawing one frame, in other words until VBlank is entered
            do { clock(); } while (!ppu->frameDone);
            ppu->frameDone = false;
            //clear the screen first
            window.clear();
            //update status, doesn't really display every change in the cpu status, but it looks cool
            updateStatus();
            //draw the Info Text and the cpu Status
            for (int i = 0; i < 8; i++) {
                window.draw(cpuStatus[i]);
                window.draw(controllerInfo[i]);
            }

            //Draw the frame and patterntables
            window.draw(mainScreen);
            window.draw(patternScreens[0]);
            window.draw(patternScreens[1]);
            //Once everything is drawn the window is displayed
            window.display();
    }
}

//Initializes the cpu Status screen
void NESemulator::initStatus() {
    /*if(!myFont.loadFromFile("resources/Fonts/ARIAL.TTF")) {
        cout << "Error Loading font (ignore when running source code)" << endl;
    }*/

        if (!myFont.loadFromFile("../resources/Fonts/ARIAL.TTF")) {
            cout << "Error loading font, shouldn't happen" << endl;
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

//Initializes the ControllerInfo text; always call as the font is loaded there atm initStatus first
void NESemulator::initControllerInfo() {
    for (int i = 0; i < 5; i++) {
        controllerInfo[i].setFont(myFont);
        controllerInfo[i].setPosition(50,  650 + i*35);
        controllerInfo[i].setCharacterSize(30);
        controllerInfo[i].setStyle(sf::Text::Bold);
        controllerInfo[i].setFillColor(sf::Color::White);
    }
    controllerInfo[0].setString("Y: A Button (JUMP)");
    controllerInfo[1].setString("X: B Button (SPRINT/ABILITY)");
    controllerInfo[2].setString("A: Select (confirms selection in Menu also pauses the game)");
    controllerInfo[3].setString("S: Start (change selection in Menu)");
    controllerInfo[4].setString("Arrow Keys: Movement");

}

//Updates the cpu status registers on screen, green for set and red for cleared
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
    //for every cpu clock the ppu is clocked 3 times
    if (masterClock%3 == 0) {
        //sometimes the cpu writes to the address 0x4014, which indicates that he want to do a dma transfer for the
        //OAM in the ppu, the ppu then sets dma bool to true, during the dma transfer the cpu is idle and is not clocked
        //until the transfer is over.
        if(bus->dma) {
            if(bus->idle) {
                //the transfer is idle for 1 cycle on even cycles and idle for 2 cycles on odd cycles;
                if (masterClock % 2 == 0) {
                    bus->idle = false;
                    std::memset(ppu->primaryOAM, 0xFF, sizeof(ppu->primaryOAM));
                }
            } else {
                //when even then its leaves the idle catch and comes back odd, so on odds we fetch the oam data from
                //the main memory and on even cycles we write the data into the oam
                if (masterClock % 2 == 1) {
                    bus->dmaData = bus->busRead((bus->dmaPage<<8) | bus->dmaAddress);
                } else {
                    ppu->primaryOAM[bus->dmaAddress] = bus->dmaData;
                    bus->dmaAddress++;
                    //dmaAddress always starts at zero, in total 256 bytes get transfered, so when the address wraps around
                    //the transfer is complete and the flags are reset for the next transfer
                    if (bus->dmaAddress == 0) {
                        bus->dma = false;
                        bus->idle = true;

                    }
                }
            }
        } else {
            cpu->clock();
        }
    }
    ppu->clock();
    masterClock++;
}

//Draws the Foreground and Background Patterntable to the screen, which is then displayed
//number 0 for foreground and 1 for background
void NESemulator::drawPatternTable(int number) {
    uint8_t patternTableNumber = number;
    uint8_t x = 0, y = 0;
    sf::Color col;
    for (int i = 0; i < 256; i++) {
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