//
// Created by spex on 13.1.2019.
//

#include <cmath>

#include "Particle.h"


Particle::Particle(float x, float y, float size = 1)
{
    location_ = sf::Vector2f(x, y);
    velocity_ = sf::Vector2f(0, 0);
    this->size = size;

    obj_ = sf::CircleShape(this->size);
    //obj_.setFillColor(sf::Color(255, 255, 255));
    obj_.setFillColor(sf::Color::White);
    obj_.setPosition(location_);
    BBox = obj_.getGlobalBounds();
}


void Particle::moveTowards(sf::Vector2f target, float multiplier)
{
    sf::Vector2f direction;
    direction = (sf::Vector2f) target - this->location();
    auto length = (float) std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));

    direction = sf::Vector2f(direction.x / length, direction.y / length);
    this->move( &direction, multiplier);
}

void Particle::update()
{
    // reduce velocity
    velocity_.x -= velocity_.x / (100 / (speed/0.5));
    velocity_.y -= velocity_.y / (100 / (speed/0.5));

    // update location
    location_ += velocity_;
    updateSize();

    x = location().x;
    y = location().y;

    obj_.setPosition(location_);
    BBox = obj_.getGlobalBounds();

}

void Particle::move(const sf::Vector2f* vector, float multiplier)
{
    // apply vector to velocity
    velocity_.x += vector->x * speed * multiplier + (((rand() % 100 +1) - 50) / 55);
    velocity_.y += vector->y * speed * multiplier + (((rand() % 100 +1) - 50) / 55);
}

void Particle::updateSize()
{
    // calculate scale factor
    float scale = (float)(fabs(velocity_.x) + fabs(velocity_.y)) / 4;

    // apply upper and lower limit
    scale = std::fmin(SCALE_MAX, std::fmax(SCALE_MIN, scale));

    // set the scale
    size_QT = scale;
    obj_.setScale(scale, scale);
}

void Particle::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    obj_.setFillColor(sf::Color(r, g, b));
}

void Particle::setSpeed(float speed)
{
    this->speed = speed;
}
