//
// Created by aqua on 10.2.2019.
//

#include "Button.h"
#include <SFML/Graphics.hpp>

unsigned int window_Height_P;
sf::Font *font_P;
std::vector<button> *buttons_P;

void initButtonData(sf::Font &font, int window_Height, std::vector<button> &buttons)
{
    window_Height_P = window_Height;
    buttons_P = &buttons;
    font_P = &font;
}
sf::Vector2i buttonSize(150,30);
void createButton(int col, int row, std::vector<std::string> optionTexts, int &target) {
    button button1;
    sf::Vector2f pos(buttonSize.x * row, window_Height_P - buttonSize.y * col);
    button1.rect.setSize((sf::Vector2f)buttonSize);
    button1.rect.setFillColor(sf::Color::Green);
    button1.rect.setPosition(pos);
    button1.rect.setOutlineColor(sf::Color::Black);
    button1.rect.setOutlineThickness(2.5);
    button1.fRect = button1.rect.getGlobalBounds();
    button1.text.setFont(*font_P);
    button1.text.setString(optionTexts[0]);
    button1.optionCount = optionTexts.size();
    button1.text_Vec = optionTexts;
    button1.text.setPosition(pos);
    button1.text.setCharacterSize(25);
    button1.text.setFillColor(sf::Color::Red);
    button1.target = &target;
    buttons_P->push_back(button1);
}