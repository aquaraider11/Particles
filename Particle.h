//
// Created by spex on 13.1.2019.
//

#ifndef SFML_PARTICLESYSTEM_PARTICLE_H
#define SFML_PARTICLESYSTEM_PARTICLE_H


#include <SFML/System.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Quadtree.h"

class Particle {
public:
    float x, y, size_QT;
    Particle(float x, float y, float size);
    bool interactionsEnabled = false;
    void update();
    void move(const sf::Vector2f* vector, float multiplier);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setSpeed(float speed);
    sf::FloatRect BBox;
    Quadtree<Particle> *parent = nullptr;


    const sf::Vector2f& location() const { return location_; }
    const sf::CircleShape& obj() const { return obj_; }


private:
    const float SCALE_MAX = 4;
    const float SCALE_MIN = 1;

    sf::Vector2f location_, velocity_;
    sf::CircleShape obj_;


    float size = 1;
    float mass = 1;
    float speed = 1;

    void updateSize();
};


#endif //SFML_PARTICLESYSTEM_PARTICLE_H
