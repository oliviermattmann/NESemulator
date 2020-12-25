//
// Created by olivi on 22.12.2020.
//

#include <iostream>
#include "Renderer.h"

//Constructor for Renderer
Renderer::Renderer() {
    initVariables();
}

//Destructor for Renderer
Renderer::~Renderer() {
    delete window;
}

//private functions

void Renderer::initVariables() {
    window = nullptr;
}

void Renderer::initWindow() {
    sf::Context context;
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "Test Window");
    window->setActive(false);
    run();
}


//public functions

void Renderer::render() {

}

void Renderer::run() {

    while(window->isOpen()) {
        while(window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    exit(0);
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window->close();
                        exit(0);
                    }
                default:
                    break;
            }
        }
        window->clear(sf::Color::Cyan);
        window->display();
    }
}

