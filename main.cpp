#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <numeric>
//#include <future>
#include <cmath>
//#include <mutex>
#include <chrono>
#include <iomanip>

#include <parallel/algorithm>

#include "Particle.h"
#include "StatDisplay/StatDisplay.h"
#include "StatDisplay/FPS.h"

#include "Quadtree.h"

#include "Button.h"
#include "customShape.h"


sf::Font font_Glob;

const float VECTOR_SPEED_MULTIPLIER = 1, GRAVITY_GLOBAL_VARIABLE = 0.2;
int GRAVITY_ENABLED = 0, DRAWQT = 0;

bool vecInUse = false, mouseButtonIsPressed = false, rightButton = false;
std::vector<Particle> particles;
sf::Vector2i mouseLocation = sf::Vector2i(0,0);
unsigned int width = 1000, height = 800;
sf::FloatRect border{-(float)width,(float)height-100,(float)width * 3,100};
Quadtree<Particle> quadtree( -10.0f, -10.0f, width + 20, height + 20, 0, 500, 8);

int threadCount = std::thread::hardware_concurrency();
std::vector<std::vector<Particle *>> subVectors;

std::vector<button> buttons;

std::vector<sf::ConvexShape> customShapes;


struct threadObj
{
    std::vector<Particle *> particles;
    std::thread th;
    bool done;
};
std::vector<threadObj *> threads;

void initializeThreading()
{
    threads.reserve(threadCount);
    for (int i = 0; i < threadCount; ++i)
    {
        std::vector<Particle*> temp;
        temp.reserve(50);
        subVectors.push_back(temp);
    }
}

void fillThreadVectors(std::vector<Particle> &input)
{
    int elements = floor(input.size() / threadCount), indexinInputVec = 0;
    std::cout << "elements = " << elements << std::endl;
    for (int i = 0; i < threadCount; ++i)
    {
        std::vector<Particle *> temp;
        if(i < threadCount-1)
        {
            for (int j = 0; j < elements; ++j)
            {
                temp.push_back(&input[j+(i*elements)]);
                indexinInputVec++;
            }
        } else
        {
            for (int k = indexinInputVec; k < input.size(); ++k)
            {
                temp.push_back(&input[k]);
            }
        }
        subVectors[i] = temp;
    }
}


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
        par.id = i;
        par.setSpeed(rng / 1000);
        particles.push_back(par);


    }
    fillThreadVectors(particles);
    vecInUse = false;
}



//int collisionCheckCount = 0, maxCollisionsOnRound = 0;
void updateParticles(Particle *par)
{
    //sf::Vector2f axisLock(1,1);
    int intersectCount = 0;
    std::vector<Particle *> QTResult = quadtree.GetObjectsAt(par->x, par->y);

    /*for (auto &i : QTResult)
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
*/
    if (mouseButtonIsPressed) {
        int reverse = rightButton ? -1 : 1;
        // move the vector
        par->moveTowards(sf::Vector2f(mouseLocation), VECTOR_SPEED_MULTIPLIER * reverse);
    }

    par->prevLocation = par->location();
    par->update();


    /*if (collisionCheckCount > maxCollisionsOnRound)
        maxCollisionsOnRound = collisionCheckCount;
    std::cout << maxCollisionsOnRound << std::endl;
    collisionCheckCount = 0;*/

}




void initCustomShapes_DEBUG()
{
    ln::customShape shape(30);
    shape.setOrigin(30,30);

    sf::ConvexShape poly;
    poly.setPointCount(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); ++i) {
        poly.setPoint(i, shape.getPoint(i));
    }

    poly.setPosition(300, 300);
    poly.setFillColor(sf::Color::Magenta);

    customShapes.push_back(poly);

}

/*void csJoin(sf::ConvexShape &shape, sf::CircleShape &brush)
{
    sf::Vector2f oldOriging = shape.getOrigin();
    shape.setOrigin(brush.getOrigin());



    shape.updateShape();
}

enum shapeEditOption {join, carve};
void updateCustomShape(sf::ConvexShape &shape, sf::CircleShape &brush, shapeEditOption option = join)
{
    switch (option)
    {
        case join: csJoin(shape, brush);
            break;
        case carve:
            break;
        default:
            std::cout << "Error" << std::endl;
    }

}*/

void updateThreads(bool *done, std::vector<Particle *> *input)
{
    std::cout << "Thread started:" << std::endl;
    while (true)
    {
        if (*done)
        {
            std::this_thread::sleep_for(3ms);
            continue;
        }
        for (auto &i : *input)
        {
            //std::cout << "Updating particles..." << std::endl;
            updateParticles(i);
        }

        *done = true;
    }
}


struct crossHair
{
    sf::CircleShape shape;
    int initialSize = 5, sizeMult;

};
crossHair CH;
void initCrosshair()
{
    CH.shape.setRadius(CH.initialSize);
    CH.shape.setFillColor(sf::Color::Transparent);
    CH.shape.setOutlineColor(sf::Color::Red);
    CH.shape.setOutlineThickness(0.1);
    CH.shape.setOrigin(CH.initialSize, CH.initialSize);


}
int old_Size;
void updateCrosshair(sf::Vector2f pos, bool clicked)
{
    CH.shape.setPosition(pos);
    CH.shape.setScale(CH.initialSize * (CH.sizeMult +1), CH.initialSize * (CH.sizeMult +1));
    if(clicked) {
        CH.shape.setOutlineColor(sf::Color::Green);
        for (int i = 0; i < CH.shape.getPointCount(); ++i) {
            //std::cout << "Point " << i << " = [" <<CH.shape.getPoint(i).x << ", " << CH.shape.getPoint(i).y << "] ";
        }
        //std::cout << std::endl;
    }
    else
        CH.shape.setOutlineColor(sf::Color::Red);


}

int desiredParticles = 1000;
void handleEvents(sf::Event *e, sf::Window *w)
{
    if (e->type == sf::Event::Closed)
        w->close();

    if (e->type == sf::Event::KeyPressed)
    {
        if (e->key.code == sf::Keyboard::Escape)
        {
            w->close();
        }
        if (e->key.code == sf::Keyboard::F5)
        {
            std::thread t1(createParticles, desiredParticles, 0.5);
            t1.detach();
        }

        if (e->key.code == sf::Keyboard::Return)
        {
            int largestNumber = 0;
            for (const auto &i : quadtree.GetLeafObjectsCount())
            {
                if (i > largestNumber)
                    largestNumber = i;
                std::cout << i << ", ";
            }
            std::cout << std::endl;
            std::cout << "Largest Number = " << largestNumber << std::endl;

        }
    }

    if (e->type == sf::Event::MouseButtonPressed || mouseButtonIsPressed)
        mouseLocation = sf::Mouse::getPosition(*w);

    if (e->type == sf::Event::MouseButtonPressed)
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

    } else if (e->type == sf::Event::MouseButtonReleased)
    {
        mouseButtonIsPressed = !mouseButtonIsPressed;
        rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    }
}

int trailLength = 500;
std::vector<sf::Texture> frames;
int main()
{
    initCustomShapes_DEBUG();
    initializeThreading();
    initCrosshair();
    initButtonData(font_Glob, height, buttons);


    std::cout << threadCount << std::endl;
    StatDisplay sd;

    FPS fps;
    sd.AddDisplay(&fps);

    sf::Text sdText;
    sdText.setFont(font_Glob);
    sdText.setFillColor(sf::Color::Green);


    int bgColor = 0;
    font_Glob.loadFromFile("arial.ttf");
    sf::RenderWindow window(sf::VideoMode(width, height), "lolkek!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(144);

    //createParticles(64, 1);
    std::vector<std::string> options1{"BGColor B", "BGColor W"};
    createButton(1,3,options1,bgColor);
    createButton(1,2,std::vector<std::string>{"Gravity OFF", "Gravity ON"}, GRAVITY_ENABLED);
    createButton(1,4,std::vector<std::string>{"Hide QT", "Draw QT"}, DRAWQT);
    createButton(1,1,std::vector<std::string>{"Cursor 10", "Cursor 20", "Cursor 30", "Cursor 40", "Cursor 50"}, CH.sizeMult);


    for (int l = 0; l < threadCount; ++l)
    {
        threadObj *thread = new threadObj;
        thread->done = false;
        thread->th = std::thread(updateThreads, &thread->done, &subVectors[l]);
        //thread->th.detach();
        threads.push_back(thread);
    }

    frames.reserve(trailLength);


    bool first(true);
    while (window.isOpen())
    {
        window.display();

        sf::Event event;
        while (window.pollEvent(event))
        {
            handleEvents(&event, &window);
        }

        window.clear(bgColor ? sf::Color::White : sf::Color::Black);

        if (vecInUse)
            continue;

        quadtree.Clear();
        for (auto &k : particles)
        {
            quadtree.AddObject( &k );
        }

       for (const auto &l : threads)
        {
            l->done = false;
        }

        int loopcount = 0;
        while (true)
        {
            std::this_thread::sleep_for(1ms);
            loopcount++;
            unsigned int completedThreads = 0;
            for (const auto &i : threads)
                if (i->done)
                    completedThreads++;

            if (completedThreads >= threadCount)
                break;
            if (loopcount > 1000)
            {
                std::cout << "Infinite Loop" << std::endl;
                window.close();
                break;
            }
            
        }

        for (const auto &m : subVectors)
        {
            for (const auto &i : m)
            {
                window.draw(i->obj());
            }
        }
        /*for (auto &i : particles)
        {
            //if (i.location() == i.prevLocation)
            //    std::cout << i.id << ", ";
            window.draw(i.obj());
            //std::cout << "(" << setfill('0') << setw(3) <<  floor(i.location().x) << "," << setfill('0') << setw(3) << floor(i.location().y) << ") ";
        }*/
        //std::cout << std::endl;
        //updateCrosshair(sf::Vector2f(sf::Mouse::getPosition(window)), sf::Mouse::isButtonPressed(sf::Mouse::Left));
        //window.draw(CH.shape);
        if (DRAWQT)
            quadtree.Draw(window);
        for (const auto &j : buttons)
        {
            window.draw(j.rect);
            window.draw(j.text);
        }

        /*for (const auto &n : customShapes)
        {
            window.draw(n);
        }*/


        sdText.setString(sd.GetText());
        window.draw(sdText);


    }
    for (const auto &m : threads)
    {
        delete(m);
    }

    return 0;
}

