#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <numeric>
#include <future>
#include <cmath>
#include <mutex>

#include <parallel/algorithm>

#include "Particle.h"
#include "StatDisplay/StatDisplay.h"
#include "StatDisplay/FPS.h"

#include "Quadtree.h"

const float VECTOR_SPEED_MULTIPLIER = 1, GRAVITY_GLOBAL_VARIABLE = 0.2;
int GRAVITY_ENABLED = 0, DRAWQT = 0;

bool vecInUse = false, mouseButtonIsPressed = false, rightButton = false;
std::vector<Particle> particles;
sf::Vector2i mouseLocation = sf::Vector2i(0,0);
unsigned int width = 1000, height = 800;
sf::FloatRect border{-(float)width,(float)height-100,(float)width * 3,100};
Quadtree<Particle> quadtree( -10.0f, -10.0f, width + 20, height + 20, 0, 500, 8);



void createParticles(int ammount, float sizeMultiplier)
{
    if (vecInUse)
        return;

    vecInUse = true;
    particles.clear();

    for (int i = 0; i < ammount; ++i)
    {
        Particle par(
                rand() % width + 1,
                rand() % height + 1 -100,
                (rand() % 10 + 1) * sizeMultiplier);

        par.setColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);

        float rng = rand() % 1000 + 1;
        if (rng < 800)
            rng += 200;

        par.setSpeed(rng / 1000);

        particles.push_back(par);


    }

    vecInUse = false;
}

sf::Vector2f calculateUnitVector(sf::Vector2f source, sf::Vector2f target)
{
    sf::Vector2f direction;
    direction = (sf::Vector2f) target - source;
    auto length = (float) std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));

    return sf::Vector2f(direction.x / length, direction.y / length);

}

//int collisionCheckCount = 0, maxCollisionsOnRound = 0;
void updateParticles(Particle *par)
{
    sf::Vector2f axisLock(1,1);
    int intersectCount = 0;
    std::vector<Particle *> QTResult = quadtree.GetObjectsAt(par->location().x, par->location().y);
    for (auto &i : QTResult)
    {
        if (par->obj().getGlobalBounds().contains(i->location()))
        {
            intersectCount++;

            par->setColor(255,0,0);
            //collisionCheckCount++;
        }
        if (i->obj().getGlobalBounds().contains(par->location()))
        {
            i->setColor(255, 0, 0);
            intersectCount++;
        }

    }
    if (intersectCount < 1)
        par->setColor(255,255,255);

    if (mouseButtonIsPressed) {
        int reverse = rightButton ? -1 : 1;
        // move the vector
        par->moveTowards(sf::Vector2f(mouseLocation), VECTOR_SPEED_MULTIPLIER * reverse);
    }

    par->update();

    /*if (collisionCheckCount > maxCollisionsOnRound)
        maxCollisionsOnRound = collisionCheckCount;
    std::cout << maxCollisionsOnRound << std::endl;
    collisionCheckCount = 0;*/

}


/*
 * Font Global variable and button structure
 * Don't optimize font global variable to be included in any function, and it's loaded as first line of main() for a reason
 * Struct button: Pretty self explanatory, contains all the data that a button needs to work
 */
sf::Font font_Glob;
struct button
{
    sf::RectangleShape rect;
    sf::FloatRect fRect;
    sf::Text text;
    std::vector<std::string> text_Vec;
    int *target, optionCount;
};

/*
 * Create button function:
 * Creating buttons is expensive DO NOT CREATE LOT OF BUTTONS
 * Gives interactable toggle buttons with simple function with few inputs with power to affect any boolean assainged to it.. unles const
 * You can change the button appearance in this function
 *
 */
std::vector<button> buttons;
sf::Vector2i buttonSize(150,30);
void createButton(int col, int row, std::vector<std::string> optionTexts, int& target)
{
    button button1;
    sf::Vector2f pos(buttonSize.x * row, height - buttonSize.y * col);
    button1.rect.setSize((sf::Vector2f)buttonSize);
    button1.rect.setFillColor(sf::Color::Green);
    button1.rect.setPosition(pos);
    button1.rect.setOutlineColor(sf::Color::Black);
    button1.rect.setOutlineThickness(2.5);
    button1.fRect = button1.rect.getGlobalBounds();
    button1.text.setFont(font_Glob);
    button1.text.setString(optionTexts[0]);
    button1.optionCount = optionTexts.size();
    button1.text_Vec = optionTexts;
    button1.text.setPosition(pos);
    button1.text.setCharacterSize(25);
    button1.text.setFillColor(sf::Color::Red);
    button1.target = &target;
    buttons.push_back(button1);

}



int main()
{
    StatDisplay sd;

    FPS fps;
    sd.AddDisplay(&fps);

    sf::Text sdText;
    sdText.setFont(font_Glob);
    sdText.setFillColor(sf::Color::Green);





    int bgColor = 0;
    font_Glob.loadFromFile("arial.ttf");
    sf::RenderWindow window(sf::VideoMode(width, height), "lolkek!");
    window.setFramerateLimit(144);

    createParticles(100, 1);



    /*
     *  Create button to change background color
     *  First 2 variables set the button location (cols, rows)
     *  Third variable takes vector of strings as input to define how many options button has and what are the texts of those buttons
     *  (std::vector<std::string> foo{"option1", "option2")
     *  If you don't want the text to change, but want multiple options, just add several same texts to above mentioned vector
     *  Last option is integer, that gets iterated by the button, and flipped to 0 when reaching foo.size() -1 of above mentioned vector
     *
     *  If only 2 options are mentioned, the int pointed in the last variable can be used as a boolean but the values are "flipped"
     *                        Function     Button         foo[1]            foo[0]
     *                          \/          \/              \/                \/
     *  for example (        window.clear(bgColor ? sf::Color::White : sf::Color::Black);       )
     *
     *  Witch is reasonable considering that boolean true is 1, and false is 0
     *
     */

    std::vector<std::string> options1{"BGColor B", "BGColor W"};
    createButton(1,3,options1,bgColor);
    createButton(1,2,std::vector<std::string>{"Gravity OFF", "Gravity ON"}, GRAVITY_ENABLED);
    createButton(1,4,std::vector<std::string>{"Hide QT", "Draw QT"}, DRAWQT);

    while (window.isOpen())
    {
        window.display();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::F5)
                {
                    std::thread t1(createParticles, 3000, 0.5);
                    t1.detach();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed || mouseButtonIsPressed)
                mouseLocation = sf::Mouse::getPosition(window);

            if (event.type == sf::Event::MouseButtonPressed)
            {
                mouseButtonIsPressed = !mouseButtonIsPressed;
                rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);
                //Seeing if the mouse is on any button
                for (auto &i : buttons)
                {
                    if (i.fRect.contains((sf::Vector2f)mouseLocation))
                    {
                        //If mouse is on a button and was clicked
                        std::cout << i.text_Vec[*i.target] << i.optionCount << std::endl;
                        std::cout << *i.target << std::endl;
                        if (*i.target < i.optionCount-1)
                            *i.target = *i.target + 1;
                        else
                            *i.target = 0;
                        i.text.setString(i.text_Vec[*i.target]);


                    }
                }

            } else if (event.type == sf::Event::MouseButtonReleased)
            {
                mouseButtonIsPressed = !mouseButtonIsPressed;
                rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);

            }


        }
        window.clear(bgColor ? sf::Color::White : sf::Color::Black);

        if (vecInUse)
            continue;
        quadtree.Clear();
        for (auto &k : particles)
        {
            quadtree.AddObject( &k );
        }

        // to prevent segfault, skip particle update
        // when populating particle vector

        // update particles
        __gnu_parallel::for_each(particles.begin(), particles.end(), [](auto &n)
        {
            updateParticles(&n);

        });
        for (auto &i : particles)
        {
            //updateParticles(&i);
            window.draw(i.obj());
        }
        if (DRAWQT)
            quadtree.Draw(window);
        for (const auto &j : buttons)
        {
            window.draw(j.rect);
            window.draw(j.text);
        }

        sdText.setString(sd.GetText());
        window.draw(sdText);
    }

    return 0;
}
