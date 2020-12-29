//
// Created by olivi on 25.12.2020.
//

#ifndef NESEMULATOR_SCREEN_H
#define NESEMULATOR_SCREEN_H
#include "SFML/Graphics.hpp"


class Screen : public sf::Drawable {
public:
    void init(uint16_t width, uint16_t height, float pixelSize, uint16_t x, uint16_t y, sf::Color initColor);
    void setPixel(size_t x, size_t y, sf::Color color);
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray vertices;
    sf::Vector2u screenSize;
    float pSize;
    uint16_t xPos;
    uint16_t yPos;

};


#endif //NESEMULATOR_SCREEN_H
