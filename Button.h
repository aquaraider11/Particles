//
// Created by aqua on 10.2.2019.
//

#ifndef SFML_PARTICLESYSTEM_BUTTON_H
#define SFML_PARTICLESYSTEM_BUTTON_H

#endif //SFML_PARTICLESYSTEM_BUTTON_H

#include <SFML/System.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

struct button
{
    sf::RectangleShape rect;
    sf::FloatRect fRect;
    sf::Text text;
    std::vector<std::string> text_Vec;
    int *target, optionCount;
};

void initButtonData(sf::Font &font, int window_Height, std::vector<button> &buttons);
void createButton(int col, int row, std::vector<std::string> optionTexts, int &target);
