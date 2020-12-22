//
// Created by olivi on 22.12.2020.
//

#ifndef NESEMULATOR_RENDERER_H
#define NESEMULATOR_RENDERER_H
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"



class Renderer {
private:
    //Variables
    sf::RenderWindow* window;
    sf::Event evnt;
    //private functions
    void initVariables();
    void initWindow();



public:
    //Constructors and Destructors
    Renderer(sf::Thread thread);
    ~Renderer();
    //public functions

    void render();
};


#endif //NESEMULATOR_RENDERER_H
