//
// Created by olivi on 22.12.2020.
//

#ifndef NESEMULATOR_RENDERER_H
#define NESEMULATOR_RENDERER_H
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"



class Renderer {
private:
    //Variables
    sf::RenderWindow* window;
    sf::Event event;
    //private functions
    void initVariables();



public:
    //Constructors and Destructors
    Renderer();
    ~Renderer();
    //public functions
    void initWindow();

    void render();
    void run();
};


#endif //NESEMULATOR_RENDERER_H
