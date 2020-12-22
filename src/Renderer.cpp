//
// Created by olivi on 22.12.2020.
//

#include "Renderer.h"

//Constructor for Renderer
Renderer::Renderer(sf::Thread thread) {
    initVariables();
    initWindow();
    thread.launch();
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
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "Test Window");
}

void

//public functions

void Renderer::render() {

}

