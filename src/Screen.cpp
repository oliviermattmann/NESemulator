//
// Created by olivi on 25.12.2020.
//

#include "Screen.h"



void Screen::init(uint16_t width, uint16_t height, float pixelSize, uint16_t xCoord, uint16_t yCoord, sf::Color initColor) {
    vertices.resize(width * height * 4);
    vertices.setPrimitiveType(sf::Quads);
    screenSize = {width, height};
    pSize = pixelSize;
    xPos = xCoord;
    yPos = yCoord;

    //initialize all pixels (squares)
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            auto ind = (x + y * screenSize.x) * 4;
            sf::Vector2f coordinates (xPos + x * pSize, yPos + y * pSize);

            /*
             * for all Corners the position and color needs to be set, the color is the same for all corners
             * 0-----1
             * |     |
             * |     |
             * 3-----2
             */
            //for corner 0
            vertices[ind].position = coordinates;
            vertices[ind].color = initColor;

            //for corner 1
            vertices[ind + 1].position = coordinates + sf::Vector2f{pSize, 0};
            vertices[ind + 1].color = initColor;

            //for corner 2
            vertices[ind + 2].position = coordinates + sf::Vector2f{pSize, pSize};
            vertices[ind + 2].color = initColor;

            //for corner 3
            vertices[ind + 3].position = coordinates + sf::Vector2f{0, pSize};
            vertices[ind + 3].color = initColor;
        }
    }
}

void Screen::setPixel(size_t x, size_t y, sf::Color color) {
    auto ind = (x + y * screenSize.x) * 4;
    //check for overflow
    if(ind+3 < vertices.getVertexCount()) {
        //for corner 0
        vertices[ind].color = color;

        //for corner 1
        vertices[ind + 1].color = color;

        //for corner 2
        vertices[ind + 2].color = color;

        //for corner 3
        vertices[ind + 3].color = color;
    }
}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(vertices, states);
}